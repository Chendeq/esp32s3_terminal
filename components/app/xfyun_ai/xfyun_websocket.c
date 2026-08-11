/**
 * @file xfyun_websocket.c
 * @brief 讯飞星火大模型 AI 对话模块 —— 传输层实现
 *
 * 实现内容：
 *   1. Spark 标准鉴权：按讯飞官方算法生成带 authorization / date / host
 *      三个签名参数的 WSS 地址（HMAC-SHA256 + 两次 base64 + URL 编码），
 *      并缓存 300 秒避免重复计算；
 *   2. 基于 esp_websocket_client 建立 TLS 加密的 WebSocket 连接；
 *   3. 单轮对话流程：连接 -> 发送请求 JSON -> 接收流式消息（分片重组）
 *      -> 逐条回调上层 -> 收到结束信号 -> 清理资源。
 *
 * 依赖：datetime（SNTP 校时）、mbedtls（HMAC / base64）、
 *       esp_websocket_client（WSS 客户端）、url_encode（URL 编码）。
 */

#include "xfyun_websocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "datetime.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "esp_websocket_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "mbedtls/base64.h"
#include "mbedtls/md.h"
#include "url_encode.h"
#include "xfyun.h"

static const char *TAG = "xfyun_ws";

/* 鉴权 URL 缓存有效期（秒）：签名基于 GMT 时间，
 * 短时间内复用可省去重复的 HMAC / base64 / URL 编码计算 */
#define XFYUN_AUTH_URL_CACHE_SEC 300

/**
 * @brief 单轮 WebSocket 对话的上下文
 *
 * 通过该结构把"同步阻塞"的 xfyun_ws_chat() 与"事件驱动"的
 * esp_websocket_client 事件回调关联起来：
 *   - 主流程阻塞等待信号量；
 *   - 事件回调（运行在 WS 客户端任务中）在合适时机释放信号量唤醒主流程。
 */
typedef struct {
    SemaphoreHandle_t connected;   /* 连接成功信号量：收到 CONNECTED 事件时释放 */
    SemaphoreHandle_t done;        /* 本轮结束信号量：完成 / 出错 / 断开 / 关闭帧时释放 */
    xfyun_ws_message_cb_t cb;      /* 上层消息回调：每收到一条完整消息回调一次 */
    void *user_ctx;                /* 透传给回调的上下文（如 parse_ctx_t*） */
    char *frame;                   /* 当前这条消息的拼接缓冲（用于分片重组） */
    int frame_size;                /* 当前消息的总字节数（payload_len） */
    bool done_ok;                  /* 是否已正常完成（回调返回 true，收到完整结果） */
    bool failed;                   /* 是否发生错误（异常断开 / 错误事件） */
} ws_ctx_t;

/* 鉴权 URL 缓存：s_cached_auth_url 为上次生成的 URL，s_cached_auth_time 为生成时刻 */
static char *s_cached_auth_url = NULL;
static time_t s_cached_auth_time = 0;

/**
 * @brief 简易 strdup：把字符串复制到新分配的堆内存（调用方负责 free）
 * @param src 源字符串，可为 NULL
 * @return 复制后的新字符串；src 为空或内存不足时返回 NULL
 */
static char *str_dup_alloc(const char *src)
{
    if (!src) {
        return NULL;
    }
    size_t len = strlen(src) + 1;    /* +1 包含结束符 '\0' */
    char *dst = malloc(len);
    if (dst) {
        memcpy(dst, src, len);
    }
    return dst;
}

/**
 * @brief 对数据做 base64 编码，结果以 '\0' 结尾（调用方负责 free）
 *
 * mbedtls_base64_encode 采用"两步式"用法：
 *   第一次传入 NULL/0 查询所需缓冲区大小，第二次再实际编码。
 *
 * @param src     待编码数据
 * @param src_len 数据长度（字节）
 * @param out     输出参数：编码结果字符串（需 free），失败时为 NULL
 * @return ESP_OK          成功
 * @return ESP_ERR_NO_MEM  内存不足
 * @return ESP_FAIL        编码失败
 */
static esp_err_t base64_encode_alloc(const unsigned char *src, size_t src_len, char **out)
{
    size_t olen = 0;
    /* 第一步：查询编码后所需字节数（结果写入 olen） */
    int ret = mbedtls_base64_encode(NULL, 0, &olen, src, src_len);
    if (ret != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
        /* 非预期返回值：若为 0 说明无需缓冲（不太可能），否则视为失败 */
        return ret == 0 ? ESP_OK : ESP_FAIL;
    }

    /* 按所需大小分配缓冲区（多留 1 字节放 '\0'） */
    unsigned char *buf = calloc(1, olen + 1);
    if (!buf) {
        return ESP_ERR_NO_MEM;
    }

    /* 第二步：实际编码 */
    if (mbedtls_base64_encode(buf, olen + 1, &olen, src, src_len) != 0) {
        free(buf);
        return ESP_FAIL;
    }
    buf[olen] = '\0';                   /* 补上字符串结束符 */
    *out = (char *)buf;
    return ESP_OK;
}

/**
 * @brief 构建带鉴权参数的 WSS 地址（讯飞星火标准鉴权算法）
 *
 * 鉴权步骤（与讯飞官方文档一致）：
 *   1. 取 UTC 当前时间，生成 RFC 1123 格式的 date（如
 *      "Sun, 10 Aug 2026 08:00:00 GMT"），此时间必须与服务器同步，
 *      因此先调用 datetime_wait_sntp_sync() 等待 SNTP 校时；
 *   2. 拼接待签名字符串：
 *        host: <XFYUN_HOST>\ndate: <date>\nGET <XFYUN_PATH> HTTP/1.1
 *   3. 用 API_SECRET 对待签名字符串做 HMAC-SHA256，结果 base64 得到 signature；
 *   4. 拼接原始授权串：
 *        api_key="<KEY>", algorithm="hmac-sha256",
 *        headers="host date request-line", signature="<signature>"
 *   5. 对原始授权串再 base64，得到 authorization；
 *   6. 将 authorization / date / host 三个参数 URL 编码后拼到 WSS 地址上。
 *
 * 结果会缓存 XFYUN_AUTH_URL_CACHE_SEC 秒，减少重复计算。
 *
 * @param out_url 输出参数：生成的 WSS 地址（需 free），失败时为 NULL
 * @return ESP_OK          成功
 * @return ESP_ERR_NO_MEM  内存不足
 * @return ESP_FAIL        签名或编码失败（如 SNTP 未同步、HMAC 初始化失败）
 */
static esp_err_t build_auth_url(char **out_url)
{
    /* 鉴权签名依赖准确的 UTC 时间，先等 SNTP 同步完成 */
    datetime_wait_sntp_sync();

    time_t now = 0;
    struct tm tm_utc = {0};
    char date[64] = {0};
    time(&now);
    /* 缓存命中：直接复制缓存 URL 返回，跳过签名计算 */
    if (s_cached_auth_url && s_cached_auth_time > 0 &&
        now >= s_cached_auth_time && (now - s_cached_auth_time) < XFYUN_AUTH_URL_CACHE_SEC) {
        *out_url = str_dup_alloc(s_cached_auth_url);
        return *out_url ? ESP_OK : ESP_ERR_NO_MEM;
    }

    /* 生成 RFC 1123 格式的 GMT 时间字符串 */
    gmtime_r(&now, &tm_utc);
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S GMT", &tm_utc);

    /* 步骤 2：拼接待签名字符串（三行，行尾为 \n） */
    char sign_src[160];
    snprintf(sign_src, sizeof(sign_src), "host: %s\ndate: %s\nGET %s HTTP/1.1",
             XFYUN_HOST, date, XFYUN_PATH);

    /* 步骤 3：HMAC-SHA256 计算签名摘要（32 字节） */
    unsigned char digest[32];
    const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if (!md || mbedtls_md_hmac(md, (const unsigned char *)XFYUN_API_SECRET,
                               strlen(XFYUN_API_SECRET), (const unsigned char *)sign_src,
                               strlen(sign_src), digest) != 0) {
        return ESP_FAIL;
    }

    /* 摘要 base64 编码得到 signature */
    char *signature = NULL;
    if (base64_encode_alloc(digest, sizeof(digest), &signature) != ESP_OK) {
        return ESP_FAIL;
    }

    /* 步骤 4：拼接原始授权串 */
    char auth_src[256];
    snprintf(auth_src, sizeof(auth_src),
             "api_key=\"%s\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"%s\"",
             XFYUN_API_KEY, signature);
    free(signature);

    /* 步骤 5：授权串整体 base64 得到 authorization */
    char *auth_b64 = NULL;
    esp_err_t ret = base64_encode_alloc((const unsigned char *)auth_src, strlen(auth_src), &auth_b64);
    if (ret != ESP_OK) {
        return ret;
    }

    /* 步骤 6：三个参数分别 URL 编码（date 含空格/冒号等需编码） */
    char *auth = url_encode_alloc(auth_b64, -1);
    char *enc_date = url_encode_alloc(date, -1);
    char *host = url_encode_alloc(XFYUN_HOST, -1);
    free(auth_b64);

    if (!auth || !enc_date || !host) {
        free(auth);
        free(enc_date);
        free(host);
        return ESP_ERR_NO_MEM;
    }

    /* 计算最终 URL 长度并拼装（+1 为结束符） */
    size_t url_len = strlen("wss://") + strlen(XFYUN_HOST) + strlen(XFYUN_PATH) +
                     strlen("?authorization=&date=&host=") + strlen(auth) +
                     strlen(enc_date) + strlen(host) + 1;
    char *url = malloc(url_len);
    if (!url) {
        free(auth);
        free(enc_date);
        free(host);
        return ESP_ERR_NO_MEM;
    }

    snprintf(url, url_len, "wss://%s%s?authorization=%s&date=%s&host=%s",
             XFYUN_HOST, XFYUN_PATH, auth, enc_date, host);
    free(auth);
    free(enc_date);
    free(host);

    /* 缓存本次结果供后续请求复用；缓存失败不影响本次使用 */
    char *cache_copy = str_dup_alloc(url);
    if (cache_copy) {
        free(s_cached_auth_url);
        s_cached_auth_url = cache_copy;
        s_cached_auth_time = now;
    }

    *out_url = url;
    return ESP_OK;
}

/**
 * @brief esp_websocket_client 事件回调（运行在 WS 客户端任务上下文中）
 *
 * 通过信号量与阻塞在 xfyun_ws_chat() 中的主流程同步：
 *   - CONNECTED：释放 connected 信号量，主流程开始发送请求；
 *   - DATA：分片重组，拼出完整消息后调用上层回调；回调返回 true 则结束；
 *   - ERROR / DISCONNECTED：置失败标志并释放 done，主流程返回错误。
 *
 * @param handler_args 指向 ws_ctx_t（通过 register_events 传入）
 * @param base         事件基准（未使用）
 * @param event_id     WEBSOCKET_EVENT_* 事件类型
 * @param event_data   事件数据（DATA 事件为 esp_websocket_event_data_t*）
 */
static void ws_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    (void)base;
    ws_ctx_t *ctx = (ws_ctx_t *)handler_args;
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;

    switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
        /* 连接建立成功，唤醒等待连接的主流程 */
        xSemaphoreGive(ctx->connected);
        break;
    case WEBSOCKET_EVENT_DATA:
        /* op_code 0x08 为 WebSocket 关闭帧：直接视为本轮结束 */
        if (data->op_code == 0x08) {
            xSemaphoreGive(ctx->done);
            break;
        }
        /*
         * 分片重组：一条大消息会被拆成多次 DATA 事件投递，
         * payload_offset 表示本次数据在整条消息中的偏移，
         * payload_len 表示整条消息的长度。
         * - payload_offset == 0：本条消息开始，重新分配拼接缓冲；
         * - 收到最后一段（offset+len == payload_len）：消息完整，回调上层。
         */
        if (data->payload_offset == 0) {
            free(ctx->frame);
            ctx->frame_size = data->payload_len;
            ctx->frame = calloc(1, ctx->frame_size + 1);
        }
        if (ctx->frame && data->payload_offset + data->data_len <= ctx->frame_size) {
            memcpy(ctx->frame + data->payload_offset, data->data_ptr, data->data_len);
            if (data->payload_offset + data->data_len == ctx->frame_size) {
                ctx->frame[ctx->frame_size] = '\0';   /* 补结束符，拼成完整字符串 */
                /* 回调返回 true 表示上层已解析完毕，结束本轮 */
                if (ctx->cb && ctx->cb(ctx->frame, ctx->user_ctx)) {
                    ctx->done_ok = true;
                    xSemaphoreGive(ctx->done);
                }
            }
        }
        break;
    case WEBSOCKET_EVENT_ERROR:
    case WEBSOCKET_EVENT_DISCONNECTED:
        /* 出错/断开：若尚未正常完成则标记失败，并唤醒主流程 */
        if (!ctx->done_ok) {
            ctx->failed = true;
        }
        xSemaphoreGive(ctx->done);
        break;
    default:
        break;
    }
}

/**
 * @brief 发起一轮 Spark WebSocket 对话（阻塞式，必须在独立任务中调用）
 *
 * 流程：构建鉴权 URL -> 创建信号量 -> 初始化并启动 WS 客户端 ->
 *       等待连接 -> 发送请求 JSON -> 等待结束信号（期间回调逐条解析）-> 清理。
 *
 * @param request_json Spark 协议请求 JSON 字符串
 * @param cb           消息回调：每收到一条完整消息回调一次，返回 true 提前结束
 * @param user_ctx     透传给回调的用户上下文
 * @return ESP_OK              对话正常结束（含回调提前结束）
 * @return ESP_ERR_INVALID_ARG 参数无效
 * @return ESP_ERR_NO_MEM      内存不足
 * @return ESP_ERR_TIMEOUT     连接超时（10s）或响应超时（120s）
 * @return ESP_FAIL            鉴权 URL 构建失败 / 客户端创建失败 / 发送失败 /
 *                             异常断开等
 */
esp_err_t xfyun_ws_chat(const char *request_json, xfyun_ws_message_cb_t cb, void *user_ctx)
{
    if (!request_json || !cb) {
        return ESP_ERR_INVALID_ARG;
    }

    /* 生成带鉴权参数的 WSS 地址 */
    char *url = NULL;
    esp_err_t ret = build_auth_url(&url);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "build auth url failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* 初始化本轮对话上下文：创建两个二进制信号量 */
    ws_ctx_t ctx = {
        .connected = xSemaphoreCreateBinary(),
        .done = xSemaphoreCreateBinary(),
        .cb = cb,
        .user_ctx = user_ctx,
    };
    if (!ctx.connected || !ctx.done) {
        free(url);
        if (ctx.connected) vSemaphoreDelete(ctx.connected);
        if (ctx.done) vSemaphoreDelete(ctx.done);
        return ESP_ERR_NO_MEM;
    }

    /* WS 客户端配置：TLS 证书走系统根证书包；关闭自动重连由本函数统一管理 */
    esp_websocket_client_config_t cfg = {
        .uri = url,
        .buffer_size = 2048,                 /* 接收缓冲区：大消息会被分片多次回调 */
        .task_stack = 6144,                  /* WS 客户端任务栈大小 */
        .disable_auto_reconnect = true,      /* 不自动重连，避免状态错乱 */
        .network_timeout_ms = 10000,         /* 网络超时 10s */
        .crt_bundle_attach = esp_crt_bundle_attach, /* 使用 ESP 根证书包校验服务器 */
    };

    esp_websocket_client_handle_t client = esp_websocket_client_init(&cfg);
    if (!client) {
        free(url);
        vSemaphoreDelete(ctx.connected);
        vSemaphoreDelete(ctx.done);
        return ESP_FAIL;
    }

    /* 注册事件回调并启动客户端 */
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, ws_event_handler, &ctx);
    ret = esp_websocket_client_start(client);
    /* 等待连接建立（最长 10s），成功后才发送请求 */
    if (ret == ESP_OK && xSemaphoreTake(ctx.connected, pdMS_TO_TICKS(10000)) == pdTRUE) {
        /* 发送请求 JSON（文本帧），最长等待 10s */
        int sent = esp_websocket_client_send_text(client, request_json, strlen(request_json), pdMS_TO_TICKS(10000));
        if (sent < 0) {
            ret = ESP_FAIL;
        } else if (xSemaphoreTake(ctx.done, pdMS_TO_TICKS(120000)) != pdTRUE) {
            /* 等待最终响应，最长 120s */
            ret = ESP_ERR_TIMEOUT;
        } else if (ctx.failed) {
            /* 收到结束信号但期间发生错误（断开/异常） */
            ret = ESP_FAIL;
        }
    } else {
        ret = ESP_ERR_TIMEOUT;
    }

    /* 收尾：停止、注销事件、销毁客户端并释放全部资源 */
    esp_websocket_client_stop(client);
    esp_websocket_unregister_events(client, WEBSOCKET_EVENT_ANY, ws_event_handler);
    esp_websocket_client_destroy(client);
    free(ctx.frame);                         /* 释放分片重组缓冲 */
    vSemaphoreDelete(ctx.connected);
    vSemaphoreDelete(ctx.done);
    free(url);                               /* 释放鉴权 URL */
    return ret;
}
