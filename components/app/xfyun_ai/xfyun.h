#ifndef __XF_YUN_H__
#define __XF_YUN_H__

/**
 * @file xfyun.h
 * @brief 讯飞星火大模型（Spark）AI 对话模块 —— 业务层配置与接口
 *
 * 模块职责：对接讯飞星火认知大模型 WebSocket API（v4.0 / 4.0Ultra），
 * 按"业务层 + 传输层"两层划分：
 *   - xfyun.c / xfyun.h          ：业务层。构造请求 JSON、解析流式响应，
 *                                  并通过 FreeRTOS 队列调度问题处理；
 *   - xfyun_websocket.c / .h     ：传输层。生成 Spark 鉴权 URL、
 *                                  建立 WSS 连接、收发消息与分片重组。
 *
 * 使用前提：
 *   1. WiFi 已连接（见 wifi.h）；
 *   2. 系统时间已通过 SNTP 同步（鉴权签名依赖 UTC 时间，见 datetime.h）。
 *
 * 安全提醒：以下宏中的 APPID / API_KEY / API_SECRET 属于讯飞开放平台账号
 * 凭据，请勿提交到公开仓库或泄露给他人。
 */

#include "esp_err.h"

/* ---------------- 讯飞开放平台账号凭据（请妥善保管） ---------------- */
#define XFYUN_APPID      "fabb3708"                          /* 应用 ID（AppID）               */
#define XFYUN_API_SECRET "ZWZkM2JlMTQ2Mjk3MDM2MTU4MjQwMzg5"  /* API 密钥：HMAC-SHA256 签名密钥 */
#define XFYUN_API_KEY    "81e3017f73e1741441ab42362b8ebacd"  /* API Key                        */

/* ------------------------- 服务地址与模型参数 ------------------------ */
#define XFYUN_HOST       "spark-api.xf-yun.com"              /* Spark 服务域名                 */
#define XFYUN_PATH       "/v4.0/chat"                        /* v4.0 对话接口路径              */
#define XFYUN_DOMAIN     "4.0Ultra"                          /* 模型域：4.0 Ultra              */

/**
 * @brief 初始化 AI 模块（创建发送问题队列）
 * @note  可重复调用；队列已存在时直接返回成功
 * @return ESP_OK          成功
 * @return ESP_ERR_NO_MEM  内存不足，队列创建失败
 */
esp_err_t xfyun_ai_init(void);

/**
 * @brief 向 AI 处理队列投递一个问题（非阻塞，可安全地在 LVGL 回调中调用）
 * @param question 问题字符串（UTF-8，最长 XFYUN_QUESTION_MAX_LEN 字节）
 * @return ESP_OK             入队成功
 * @return ESP_ERR_INVALID_ARG 参数为空或非法
 * @return ESP_ERR_NO_MEM     队列创建失败
 * @return ESP_ERR_TIMEOUT    队列已满（上一次请求仍在处理中）
 */
esp_err_t xfyun_ai_send_question(const char *question);

/**
 * @brief AI 处理任务入口（FreeRTOS 任务函数，由 app_task.c 创建）
 * @param pvParameters 任务参数（未使用，传 NULL）
 *
 * 循环流程：阻塞等待队列 -> 检查 WiFi -> 构造请求 JSON ->
 * 调用传输层 xfyun_ws_chat() 完成一轮对话 -> 将结果刷新到 UI。
 * 注意：本任务为串行处理，一次只处理一个问题。
 */
void xfyun_ai_task(void *pvParameters);

#endif
