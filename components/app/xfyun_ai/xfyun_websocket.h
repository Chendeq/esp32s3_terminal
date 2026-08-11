#ifndef __XFYUN_WEBSOCKET_H__
#define __XFYUN_WEBSOCKET_H__

/**
 * @file xfyun_websocket.h
 * @brief 讯飞星火大模型 AI 对话模块 —— 传输层接口
 *
 * 本层负责 Spark WebSocket API 的底层通信：
 *   - 生成带鉴权参数的 WSS 地址（HMAC-SHA256 签名，见 xfyun_websocket.c）；
 *   - 建立 TLS 加密的 WebSocket 连接；
 *   - 发送请求 JSON、接收流式响应消息并进行分片重组；
 *   - 通过回调把每一条完整消息交给上层（业务层）解析。
 *
 * 上层只需调用 xfyun_ws_chat() 并提供一个消息回调，即可完成一轮对话。
 */

#include <stdbool.h>
#include "esp_err.h"

/**
 * @brief 消息回调函数类型：每收到一条完整的 WebSocket 文本消息时被调用
 *
 * @param message  消息内容（以 '\0' 结尾，仅在本次回调执行期间有效，
 *                 如需长期保存请自行拷贝）
 * @param user_ctx 调用 xfyun_ws_chat() 时透传的用户上下文
 * @return true   表示本轮对话可以结束（例如解析到 status==2 或业务错误）
 * @return false  表示还需继续接收后续消息
 */
typedef bool (*xfyun_ws_message_cb_t)(const char *message, void *user_ctx);

/**
 * @brief 发起一轮 Spark WebSocket 对话（阻塞式，必须在独立任务中调用）
 *
 * @param request_json Spark 协议请求 JSON 字符串（构造见 xfyun.c 的
 *                     build_request_json()）
 * @param cb           消息回调：每收到一条完整消息回调一次；
 *                     回调返回 true 时提前结束本轮对话
 * @param user_ctx     透传给回调的用户上下文（如 parse_ctx_t*）
 *
 * @return ESP_OK              对话正常结束（含回调返回 true 提前结束）
 * @return ESP_ERR_INVALID_ARG 参数无效（request_json 或 cb 为空）
 * @return ESP_ERR_NO_MEM      内存不足
 * @return ESP_ERR_TIMEOUT     连接 / 发送 / 响应超时
 * @return ESP_FAIL            其他错误（鉴权 URL 构建失败、连接失败、
 *                             发送失败、异常断开等）
 *
 * @note 阻塞时长：等待连接最长 10s，等待最终响应最长 120s，
 *       期间所在任务被挂起，切勿在 LVGL 事件回调或中断上下文中调用。
 */
esp_err_t xfyun_ws_chat(const char *request_json, xfyun_ws_message_cb_t cb, void *user_ctx);

#endif
