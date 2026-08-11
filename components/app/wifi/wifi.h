#ifndef __WIFI_H__
#define __WIFI_H__

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define WIFI_SCAN_MAX_NUM 10        //最多保存10个扫描到的WiFi到列表显示

/* WIFI指令枚举 */
typedef enum {
    WIFI_CMD_CONNECT = 0,   //请求连接WIFI
    WIFI_CMD_DISCONNECT,    //请求断开连接WIFI
    WIFI_CMD_REFRESH,       //请求扫描WIFI
} wifi_cmd_code_t;

/* WIFI信息结构体 */
typedef struct {
    char ssid[33];          //待连接WIFI的名称
    char scan_result[1024]; //扫描结果字符串
    char password[64];      //待连接WIFI的密码    
} wifi_cfg_t;

/* WiFi 状态枚举（回调通知） */
typedef enum {
    WIFI_EVT_CONNECTED,      // 已连接
    WIFI_EVT_DISCONNECTED,   // 未连接
    WIFI_EVT_SCAN_DONE,      // 扫描完成
} wifi_event_id_t;

extern QueueHandle_t g_wifi_cmd_queue;
extern wifi_cfg_t wifi_req;

/* WiFi 事件回调函数类型指针 */
typedef void (*pf_wifi_event_cb)(wifi_event_id_t event_id);

/* WiFi 初始化 */
void wifi_init(void);

/* 发送指令到 WiFi 任务 */
BaseType_t wifi_send_cmd(wifi_cmd_code_t cmd);

/* 注册事件回调函数 */
void wifi_register_handler(pf_wifi_event_cb handler);

/* 获取 WIFI 连接状态 */
bool wifi_connect_state(void);

/* 获取 WIFI 连接 SSID */
char *wifi_connect_ssid(void);

/* 连接到 WIFI */
void wifi_connect(void);

/* 断开 WIFI 连接 */
void wifi_disconnect(void);

/* 获取用户主动断开连接状态 */
bool wifi_is_user_disconnect(void);

/* 扫描 WIFI 网络 */
void wifi_scan(void);

/* 设置自动连接开关状态 */
void wifi_set_auto_connect_enabled(bool enable);

/* 获取自动连接开关状态 */
bool wifi_is_auto_connect_enabled(void);

/* 自动连接上次连接的WIFI */
bool wifi_auto_connect(void);

#endif