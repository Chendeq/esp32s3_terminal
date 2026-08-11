/**
 * @file xfyun.c
 * @brief 讯飞星火大模型 AI 对话模块 —— 业务层实现
 *
 * 本文件实现 AI 对话的业务逻辑：
 *   1. 通过 FreeRTOS 队列接收来自 UI 层的问题（xfyun_ai_send_question）；
 *   2. 在独立任务 xfyun_ai_task() 中串行处理每一个问题；
 *   3. 使用 cJSON 构造 Spark 协议请求 JSON、解析流式响应；
 *   4. 通过 app_ui.h 提供的接口把回答实时刷新到 LVGL 界面。
 *
 * 请求/响应均遵循讯飞星火 v4.0 WebSocket 协议，
 * 底层通信（鉴权 + WSS 收发）由 xfyun_websocket.c 负责。
 */

#include "xfyun.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "app_ui.h"
#include "cJSON.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "xfyun_websocket.h"
#include "wifi.h"

/* 单条问题允许的最大长度（字节），超出部分会被截断 */
#define XFYUN_QUESTION_MAX_LEN 1000

static const char *TAG = "xfyun";

/*
 * 发送问题队列（深度为 2）：
 *   - 生产者：UI 事件回调（events_init.c）调用 xfyun_ai_send_question() 入队；
 *   - 消费者：xfyun_ai_task() 阻塞等待并逐条处理。
 * 深度为 2 表示最多允许排队 2 条问题，超出时入队立即失败并返回
 * ESP_ERR_TIMEOUT（UI 提示 "Busy"），避免无界积压。
 */
static QueueHandle_t s_ai_queue = NULL;

/**
 * @brief 队列消息体：问题在入队时被深拷贝到这里
 * @note  +1 用于存放字符串结束符 '\0'；
 *        strncpy 不保证补 '\0'，需要调用方提前将结构清零
 */
typedef struct {
    char question[XFYUN_QUESTION_MAX_LEN + 1];
} ai_msg_t;

/**
 * @brief 流式响应解析上下文（由 xfyun_ai_task() 创建并传给传输层回调）
 * @note  has_content 记录本轮是否已收到过有效回答内容，
 *        用于区分"空响应"与"正常完成"
 */
typedef struct {
    bool has_content;
} parse_ctx_t;

/**
 * @brief 构建 Spark v4.0 对话请求 JSON 字符串（调用方需 free 返回值）
 *
 * 协议结构：
 *   header    : app_id（应用 ID）、uid（用户标识）
 *   parameter : chat.domain（模型域）、chat.temperature（采样温度）、
 *               chat.max_tokens（最大生成 token 数）
 *   payload   : message.text[]，仅包含一条 role="user" 的当前问题
 *
 * @param question 用户问题字符串（UTF-8）
 * @return 成功返回 malloc 分配的 JSON 字符串（调用方 free）；
 *         失败返回 NULL（如内存不足）
 */
static char *build_request_json(const char *question)
{
    /* 创建根对象；中间任一步骤失败时由 cJSON 内部处理，最后统一释放 */
    cJSON *root = cJSON_CreateObject();

    /* header：请求标识 */
    cJSON *header = cJSON_AddObjectToObject(root, "header");
    cJSON_AddStringToObject(header, "app_id", XFYUN_APPID);     /* 应用 ID     */
    cJSON_AddStringToObject(header, "uid", "esp32s3_terminal"); /* 用户标识    */

    /* parameter：模型参数 */
    cJSON *parameter = cJSON_AddObjectToObject(root, "parameter");
    cJSON *chat = cJSON_AddObjectToObject(parameter, "chat");
    cJSON_AddStringToObject(chat, "domain", XFYUN_DOMAIN);      /* 模型域：4.0Ultra */
    cJSON_AddNumberToObject(chat, "temperature", 0.5);          /* 采样温度：越低回答越保守 */
    cJSON_AddNumberToObject(chat, "max_tokens", 2048);          /* 单次最多生成的 token 数 */

    /* payload：对话消息内容（本次仅携带当前问题，不包含历史上下文） */
    cJSON *payload = cJSON_AddObjectToObject(root, "payload");
    cJSON *message = cJSON_AddObjectToObject(payload, "message");
    cJSON *text = cJSON_AddArrayToObject(message, "text");
    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "role", "user");              /* 消息角色：用户 */
    cJSON_AddStringToObject(user, "content", question);         /* 消息内容：问题 */
    cJSON_AddItemToArray(text, user);                           /* 追加到消息数组 */

    /* 输出无空格的紧凑 JSON 字符串，由调用方负责 free */
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);                                         /* 释放 cJSON 对象树 */
    return json;
}

/**
 * @brief 解析 Spark 流式响应中的一条消息（由传输层逐条回调）
 *
 * Spark 以流式方式分多条 WebSocket 消息返回结果，每条消息的结构：
 *   header : { code, status, message }        status: 2 表示最后一条
 *   payload: { choices: { text: [ { content } ] } }
 *
 * @param json     当前这条响应的 JSON 字符串（传输层已拼接完整）
 * @param user_ctx 指向 parse_ctx_t 的上下文（记录是否已有内容）
 * @return true  表示本轮对话结束（业务错误，或 status==2 最后一条）
 * @return false 表示还有后续消息，继续等待
 */
static bool parse_response(const char *json, void *user_ctx)
{
    parse_ctx_t *ctx = (parse_ctx_t *)user_ctx;
    bool done = false;                     /* 本轮是否结束 */
    cJSON *root = cJSON_Parse(json);
    if (!root) {
        /* JSON 解析失败：不结束本轮，继续等待下一条消息 */
        return false;
    }

    cJSON *header = cJSON_GetObjectItem(root, "header");
    cJSON *code = cJSON_GetObjectItem(header, "code");
    cJSON *status = cJSON_GetObjectItem(header, "status");
    /* code != 0 表示业务层错误（鉴权失败、余额不足等），直接展示错误信息并结束 */
    if (cJSON_IsNumber(code) && code->valueint != 0) {
        cJSON *msg = cJSON_GetObjectItem(header, "message");
        ai_ui_set_answer(cJSON_IsString(msg) ? msg->valuestring : "Spark response error");
        done = true;
        goto out;
    }

    cJSON *payload = cJSON_GetObjectItem(root, "payload");
    cJSON *choices = cJSON_GetObjectItem(payload, "choices");
    cJSON *texts = cJSON_GetObjectItem(choices, "text");
    if (cJSON_IsArray(texts)) {
        cJSON *item = NULL;
        /* 遍历本消息中的文本片段，把内容增量追加到 UI，实现"打字机"式流式显示 */
        cJSON_ArrayForEach(item, texts) {
            cJSON *content = cJSON_GetObjectItem(item, "content");
            if (cJSON_IsString(content) && content->valuestring[0] != '\0') {
                /* 第一条有效内容：先清掉占位提示（"Requesting..."）再开始追加 */
                if (!ctx->has_content) {
                    ai_ui_set_answer("");
                    ctx->has_content = true;
                }
                ai_ui_append_answer(content->valuestring);
            }
        }
    }

    /* status == 2 表示这是最后一条消息，通知传输层结束本轮 */
    done = cJSON_IsNumber(status) && status->valueint == 2;

out:
    cJSON_Delete(root);                    /* 释放本条 JSON */
    return done;
}

/**
 * @brief 初始化 AI 模块：创建发送问题队列
 * @note  可重复调用，具备幂等性
 * @return ESP_OK          成功（队列已存在或创建成功）
 * @return ESP_ERR_NO_MEM  队列创建失败（内存不足）
 */
esp_err_t xfyun_ai_init(void)
{
    if (s_ai_queue == NULL) {
        /* 创建深度为 2 的队列，每条消息为 ai_msg_t（问题字符串） */
        s_ai_queue = xQueueCreate(2, sizeof(ai_msg_t));
        if (!s_ai_queue) {
            return ESP_ERR_NO_MEM;
        }
    }
    return ESP_OK;
}

/**
 * @brief 发送问题到 AI 处理队列（非阻塞，可被 LVGL 事件回调直接调用）
 * @param question 问题字符串
 * @return ESP_OK            入队成功
 * @return ESP_ERR_INVALID_ARG 问题为空
 * @return ESP_ERR_NO_MEM    队列创建失败
 * @return ESP_ERR_TIMEOUT   队列已满（上一次请求还在处理中）
 */
esp_err_t xfyun_ai_send_question(const char *question)
{
    if (!question || question[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }
    /* 队列未创建时自动初始化（懒加载） */
    if (!s_ai_queue && xfyun_ai_init() != ESP_OK) {
        return ESP_ERR_NO_MEM;
    }

    ai_msg_t msg = {0};
    /* 深拷贝问题内容到队列消息，避免引用调用方的临时缓冲区 */
    strncpy(msg.question, question, sizeof(msg.question) - 1);
    /* 0 表示不等待：队列满立即返回，由调用方决定是否提示 "Busy" */
    if (xQueueSend(s_ai_queue, &msg, 0) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }
    return ESP_OK;
}

/**
 * @brief AI 处理任务主循环（由 app_task.c 的 create_ai_task() 创建）
 *
 * 任务创建参数（见 app_task.c）：
 *   - 栈大小 16KB，优先级 2，固定在 Core 0 运行，栈使用 PSRAM 分配。
 * 说明：
 *   - 本任务串行处理问题，一次仅处理一条；
 *   - xfyun_ws_chat() 是阻塞调用，最长可能阻塞约 2 分钟（含 120s 响应
 *     等待），因此必须放在独立任务中，不能放在 LVGL 事件回调里执行。
 *
 * @param pvParameters 任务参数（未使用，传 NULL）
 */
void xfyun_ai_task(void *pvParameters)
{
    (void)pvParameters;
    ai_msg_t msg;

    while (1) {
        /* 阻塞等待队列中的问题；portMAX_DELAY 下正常不会走到 continue */
        if (xQueueReceive(s_ai_queue, &msg, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        /* 发送前检查 WiFi：未连接则直接提示，避免长时间无谓等待 */
        if (!wifi_connect_state()) {
            ai_ui_set_answer("WiFi not connected");
            continue;
        }

        /* 构造 Spark 请求 JSON */
        char *request = build_request_json(msg.question);
        if (!request) {
            ai_ui_set_answer("Build request failed");
            continue;
        }

        parse_ctx_t ctx = {0};
        /* 先显示占位提示，收到首条有效内容时会被清空 */
        ai_ui_set_answer("Requesting...");
        /* 阻塞发起 WebSocket 对话；期间流式结果通过 parse_response 刷新到 UI */
        esp_err_t ret = xfyun_ws_chat(request, parse_response, &ctx);
        free(request);                       /* 释放请求 JSON */

        if (ret != ESP_OK) {
            /* 传输层失败（超时 / 断开 / 发送失败等） */
            ESP_LOGE(TAG, "chat failed: %s", esp_err_to_name(ret));
            ai_ui_set_answer("Request failed");
        } else if (!ctx.has_content) {
            /* 通信成功但没有任何有效回答内容 */
            ai_ui_set_answer("No response");
        }
    }
}
