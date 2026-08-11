#include "wifi.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs_flash.h"

static const char *TAG = "wifi";

#define WIFI_NVS_NAMESPACE "wifi_config"     // WIFI 配置 NVS 命名空间
#define NVS_KEY_AUTO_CONNECT "auto_conn"     // 自动连接开关 NVS 键名

wifi_cfg_t wifi_req = {
    .ssid = "",
    .scan_result = "",
    .password = "",
};

/* =================== 内部变量 =================== */

static bool s_is_connected = false;                 // 是否已连接
static bool s_auto_connect_enabled = false;         // 自动连接开关状态
static bool s_user_disconnect = false;              // 用户主动断开标志
static pf_wifi_event_cb s_pf_event_handler = NULL;
static char s_saved_ssid[33] = {0};                 // 保存的SSID缓存，用于判断是否需要保存当前连接的SSID

// WIFI指令队列句柄
QueueHandle_t g_wifi_cmd_queue;

/* ======================= 内部函数 ======================= */

/**
 * @brief 发送事件给 UI 层
 * @param id 事件 ID
 * @param data 事件数据指针
 * @param len 事件数据长度
 *
 */
static void wifi_send_event(wifi_event_id_t id)
{
    if (s_pf_event_handler)
    {
        s_pf_event_handler(id);
    }
}

/**
 * @brief 保存当前所连接WIFI的SSID和密码到NVS
 */
static void wifi_save_current_credentials(void)
{
    if(!s_auto_connect_enabled)             // 如果自动连接开关未启用，不保存当前连接的SSID和密码
    {
        ESP_LOGI(TAG, "Auto connect disabled, not saving credentials");
        return;
    }
    nvs_handle_t nvs;                        // NVS句柄
    esp_err_t err = nvs_open(WIFI_NVS_NAMESPACE, NVS_READWRITE, &nvs); // 打开NVS命名空间
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to open NVS for write: %s", esp_err_to_name(err));
        return;
    }
    err = nvs_set_str(nvs, "ssid", wifi_req.ssid);                  // 保存SSID到NVS
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to save SSID: %s", esp_err_to_name(err));
    }
    err = nvs_set_str(nvs, "password", wifi_req.password);          // 保存密码到NVS
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to save password: %s", esp_err_to_name(err));
    }
    nvs_commit(nvs);                                                // 提交NVS写操作
    nvs_close(nvs);                                                // 关闭NVS句柄
    strncpy(s_saved_ssid, wifi_req.ssid, sizeof(s_saved_ssid) - 1); // 复制当前连接的SSID到保存的SSID缓存
    ESP_LOGI(TAG, "Saved credentials: SSID=%s", wifi_req.ssid);
}

/**
 * @brief 从NVS中读取保存的WIFI SSID和密码
 */
static bool wifi_load_credentials(void)
{
    nvs_handle_t nvs;                        // NVS句柄
    esp_err_t err = nvs_open(WIFI_NVS_NAMESPACE, NVS_READONLY, &nvs);   // 打开NVS命名空间
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "No saved credentials (NVS open failed)");
        return false;
    }
    char ssid[33] = {0};
    char password[65] = {0};
    size_t len = sizeof(ssid);
    err = nvs_get_str(nvs, "ssid", ssid, &len);                  // 从NVS中读取SSID
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "No saved SSID");
        nvs_close(nvs);
        return false;
    }
    len = sizeof(password);
    err = nvs_get_str(nvs, "password", password, &len);          // 从NVS中读取密码
    nvs_close(nvs);
    if (err != ESP_OK) {
        password[0] = '\0';
    }
    strncpy(wifi_req.ssid, ssid, sizeof(wifi_req.ssid) - 1);                // 复制SSID到wifi_req.ssid    
    strncpy(wifi_req.password, password, sizeof(wifi_req.password) - 1);    // 复制密码到wifi_req.password
    strncpy(s_saved_ssid, wifi_req.ssid, sizeof(s_saved_ssid) - 1);         // 复制当前连接的SSID到保存的SSID缓存
    ESP_LOGI(TAG, "Loaded credentials: SSID=%s", wifi_req.ssid);
    return true;
}

/**
 * @brief 清除 NVS 中保存的 WIFI SSID和密码
 */
static void wifi_clear_credentials(void)
{
    nvs_handle_t nvs;
    if (nvs_open(WIFI_NVS_NAMESPACE, NVS_READWRITE, &nvs) == ESP_OK) 
    {
        nvs_erase_key(nvs, "ssid");
        nvs_erase_key(nvs, "password");
        nvs_commit(nvs);
        nvs_close(nvs);
    }
    memset(s_saved_ssid, 0, sizeof(s_saved_ssid));
    memset(wifi_req.ssid, 0, sizeof(wifi_req.ssid));
    memset(wifi_req.password, 0, sizeof(wifi_req.password));
    ESP_LOGI(TAG, "Cleared WiFi credentials from NVS and memory");
}

/**
 * @brief WiFi 事件处理回调（底层事件）
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            // 不开启自动连接
            break;
        case WIFI_EVENT_STA_CONNECTED:
        {
            break;
        }
        case WIFI_EVENT_STA_DISCONNECTED:
        {
            wifi_event_sta_disconnected_t *disconnect = (wifi_event_sta_disconnected_t *)event_data;
            ESP_LOGW(TAG, "断开连接,原因: %d", disconnect->reason);
            s_is_connected = false;

            wifi_send_event(WIFI_EVT_DISCONNECTED); // 通知断开连接
            break;
        }
        }
    }
    else if (event_base == IP_EVENT)
    {
        if (event_id == IP_EVENT_STA_GOT_IP)
        {
            s_is_connected = true;
            ESP_LOGI(TAG, "获取到WIFI IP,连接成功");
            // 仅当SSID与缓存不同时才保存到NVS
            if (strcmp(wifi_req.ssid, s_saved_ssid) != 0)
            {
                wifi_save_current_credentials();
            }
            wifi_send_event(WIFI_EVT_CONNECTED); // 通知连接成功
        }
    }
}

/* ========================== END ========================== */

/* ======================= 外部接口函数 ======================= */

/**
 * @brief WiFi 初始化
 */
void wifi_init(void)
{
    g_wifi_cmd_queue = xQueueCreate(5, sizeof(wifi_cmd_code_t));
    if (g_wifi_cmd_queue == NULL)
    {
        ESP_LOGE(TAG, "创建队列失败");
        return;
    }

    // 初始化网络接口
    ESP_ERROR_CHECK(esp_netif_init());

    // 创建默认事件循环
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // 初始化WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册事件处理函数
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    // 设置WiFi模式、启动WiFi
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // 读取自动连接开关状态
    nvs_handle_t nvs;
    if (nvs_open(WIFI_NVS_NAMESPACE, NVS_READONLY, &nvs) == ESP_OK) 
    {
        uint8_t val = 0;
        if (nvs_get_u8(nvs, NVS_KEY_AUTO_CONNECT, &val) == ESP_OK) 
        {
            s_auto_connect_enabled = (val != 0);
        }
        nvs_close(nvs);
    }
}

/**
 * @brief 发送指令到 WiFi 任务队列
 * @param cmd 指令指针
 * @return pdPASS 成功发送, pdFALSE 失败
 */
BaseType_t wifi_send_cmd(wifi_cmd_code_t cmd)
{
    if (g_wifi_cmd_queue == NULL)
        return pdFALSE;
    return xQueueSendToBack(g_wifi_cmd_queue, &cmd, pdMS_TO_TICKS(10)); 
}

/**
 * @brief 注册事件回调函数
 * @param handler 事件处理函数指针
 */
void wifi_register_handler(pf_wifi_event_cb handler)
{
    s_pf_event_handler = handler;
}

/**
 * @brief 获取 WIFI 连接状态
 * @return true 已连接, false 未连接
 */
bool wifi_connect_state(void)
{
    return s_is_connected;
}

/**
 * @brief 获取 WIFI 连接 SSID
 * @return char* 连接 SSID 字符串指针
 */
char *wifi_connect_ssid(void)
{
    return wifi_req.ssid;
}

/**
 * @brief 连接到 WIFI
 */
void wifi_connect(void)
{
    s_user_disconnect = false;
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

    strncpy((char *)wifi_config.sta.ssid, wifi_req.ssid, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char *)wifi_config.sta.password, wifi_req.password, sizeof(wifi_config.sta.password) - 1);

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_connect();
}

/**
 * @brief 断开 WIFI 连接
 */
void wifi_disconnect(void)
{
    s_user_disconnect = true;
    esp_wifi_disconnect();
}

/**
 * @brief 扫描 WIFI 网络
 */
void wifi_scan(void)
{
    ESP_LOGI(TAG, "Starting WiFi scan...");

    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true};

    esp_wifi_scan_start(&scan_config, true);

    uint16_t ap_num = WIFI_SCAN_MAX_NUM;
    wifi_ap_record_t ap_records[WIFI_SCAN_MAX_NUM];
    memset(ap_records, 0, sizeof(ap_records));

    esp_wifi_scan_get_ap_records(&ap_num, ap_records);

    ESP_LOGI(TAG, "Scan done, AP count = %d", ap_num);
    // 构建扫描结果
    wifi_req.scan_result[0] = '\0';
    if (ap_num == 0)
    {
        strcpy(wifi_req.scan_result, "附近无WiFi网络\n");
    }
    else
    {
        int offset = 0;
        for (int i = 0; i < ap_num && offset < (int)sizeof(wifi_req.scan_result) - 2; i++)
        {
            if (ap_records[i].ssid[0] == '\0')
                continue;
            int len = snprintf(wifi_req.scan_result + offset, sizeof(wifi_req.scan_result) - offset, "%s\n", ap_records[i].ssid);
            if (len > 0)
            {
                offset += len;
            }
            else
            {
                break;
            }
        }
        // 确保以 \0 结尾
        wifi_req.scan_result[sizeof(wifi_req.scan_result) - 1] = '\0';
    }
    wifi_send_event(WIFI_EVT_SCAN_DONE); // 通知扫描完成
}

/**
 * @brief 设置自动连接开关状态
 * @param enabled 是否启用自动连接
 */
void wifi_set_auto_connect_enabled(bool enabled)
{
    if (s_auto_connect_enabled == enabled) 
        return;
    s_auto_connect_enabled = enabled;
    nvs_handle_t nvs;
    if (nvs_open(WIFI_NVS_NAMESPACE, NVS_READWRITE, &nvs) == ESP_OK) 
    {
        nvs_set_u8(nvs, NVS_KEY_AUTO_CONNECT, enabled ? 1 : 0);
        nvs_commit(nvs);
        nvs_close(nvs);
    }
    if (!enabled) 
    {
        wifi_clear_credentials();   // 关闭开关时清除保存的凭证
    }
}

/**
 * @brief 获取自动连接开关状态
 * @return true 已启用自动连接, false 未启用自动连接
 */
bool wifi_is_auto_connect_enabled(void)
{
    return s_auto_connect_enabled;
}

/**
 * @brief 获取用户主动断开连接状态
 * @return true 用户主动断开连接, false 未主动断开连接
 */
bool wifi_is_user_disconnect(void)
{
    return s_user_disconnect;
}

/**
 * @brief 自动连接 WIFI
 * @return true 存在保存的凭证并已发送连接命令, false 不存在凭证或发送失败
 */
bool wifi_auto_connect(void)
{
    if (!s_auto_connect_enabled) 
    {
        ESP_LOGI(TAG, "Auto connect disabled, skip");
        return false;
    }
    if (wifi_load_credentials()) 
    {
        wifi_send_cmd(WIFI_CMD_CONNECT);
        return true;
    }
    return false;
}

/* ========================== END ========================== */
