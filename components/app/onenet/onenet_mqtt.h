#ifndef ONENET_MQTT_H
#define ONENET_MQTT_H

#include <stdbool.h>

#include "esp_err.h"

/* OneNET 产品与设备凭据 */
#define ONENET_PRODUCT_ID "00J2n3hecf"
#define ONENET_ACCESS_KEY "77Gn80MN37+dGeXfj7WTy8vbnbdxD035OeWjyxXBV0s="
#define ONENET_DEVICE_NAME "terminal_ota"

/* OneNET MQTT 服务地址 */
#define ONENET_MQTT_BROKER_URI "mqtts://mqtts.heclouds.com:8883"

/* Token 过期时间：2030-12-30 04:00:00 UTC */
#define ONENET_TOKEN_EXPIRE_TIME 1924833600U

/**
 * @brief 属性设置回调
 *
 * 回调在 MQTT 事件任务中运行，JSON 指针仅在回调期间有效。
 *
 * @param request_json 以 '\0' 结尾的属性设置 JSON
 * @return 返回 ESP_OK 时回复 200，否则回复 500
 */
typedef esp_err_t (*onenet_property_set_handler_t)(const char *request_json);

/**
 * @brief 创建并启动 OneNET MQTT 客户端
 *
 * 应在 Wi-Fi 获取 IP 后调用。连接恢复后会自动重新订阅主题。
 *
 * @return 成功返回 ESP_OK，否则返回 ESP-IDF 错误码
 */
esp_err_t onenet_start(void);

/**
 * @brief 停止并销毁 OneNET MQTT 客户端
 */
esp_err_t onenet_stop(void);

/**
 * @brief 判断 MQTT 当前是否已连接
 */
bool onenet_is_connected(void);

/**
 * @brief 注册属性设置消息处理函数
 *
 * 传入 NULL 可注销已有处理函数。
 */
void onenet_register_property_set_handler(onenet_property_set_handler_t handler);

/**
 * @brief 向 OneNET 上报属性数据
 *
 * @param data 完整 JSON，例如：
 *             {"id":"1","version":"1.0","params":{"temperature":25}}
 */
esp_err_t onenet_post_property_data(const char *data);

#endif /* ONENET_MQTT_H */
