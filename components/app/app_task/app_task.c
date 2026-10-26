#include "app_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/idf_additions.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "lvgl_port.h"
#include "wifi.h"
#include "app_ui.h"
#include "datetime.h"   
#include "weather.h"
#include "serial.h"
#include "music.h"
#include "mic.h"
#include "xfyun.h"
#include "onenet_mqtt.h"


static const char *TAG = "app_task";

//WIFI相关变量定义
static TaskHandle_t s_wifi_task_handle = NULL;

// 时间相关变量定义
static TaskHandle_t s_time_task_handle = NULL;

// 天气相关变量定义
static TaskHandle_t s_weather_task_handle = NULL;
#define WEATHER_REFRESH_INTERVAL_MS (30 * 60 * 1000)     // 30分钟刷新一次天气

// 串口接收相关变量定义
static TaskHandle_t s_serial_task_handle = NULL;
#define SERIAL_RX_TASK_STACK_SIZE 4096
#define SERIAL_RX_CHUNK_SIZE 128

// 音乐相关变量定义
static TaskHandle_t s_music_task_handle = NULL;

// AI相关变量定义
static TaskHandle_t s_ai_task_handle = NULL;

// OneNET MQTT startup task
static TaskHandle_t s_onenet_task_handle = NULL;


/* -------------------- WIFI任务实现（core 0） -------------------- */

/**
 * @brief WiFi任务函数
 * @param pvParameters 指令队列句柄
 */
static void wifi_task(void *pvParameters)
{
    QueueHandle_t cmd_queue = (QueueHandle_t)pvParameters;
    wifi_cmd_code_t wifi_cmd;
    TickType_t last_auto_check = xTaskGetTickCount();
    const TickType_t auto_check_interval = pdMS_TO_TICKS(10000); // 10秒检查一次，可调整
    
    while (1) 
    {
        if (xQueueReceive(cmd_queue, &wifi_cmd, auto_check_interval) == pdTRUE) 
        {
            switch (wifi_cmd) 
            {
                case WIFI_CMD_CONNECT:                 
                    // 连接到 WIFI
                    wifi_connect();
                    break;
                case WIFI_CMD_DISCONNECT:
                    wifi_disconnect();
                    break;
                case WIFI_CMD_REFRESH:
                    esp_wifi_scan_stop();

                    // 扫描 WIFI 网络
                    wifi_scan();
                    break;
                default:
                    break;
            }
        }
        // 周期性自动连接检查
        if (xTaskGetTickCount() - last_auto_check >= auto_check_interval)
        {
            last_auto_check = xTaskGetTickCount();
            // 仅在未连接且自动连接开关打开时尝试自动连接
            if (!wifi_connect_state() && wifi_is_auto_connect_enabled() && !wifi_is_user_disconnect())
            {
                wifi_auto_connect();
            }
        }
    }
}

/**
 * @brief 创建WiFi任务
 */
static void create_wifi_task(void)
{
    // 初始化WiFi
    wifi_init();
    
    if(s_wifi_task_handle == NULL)
    {
        BaseType_t ret = xTaskCreatePinnedToCore(wifi_task, 
                                                    "wifi_task", 
                                                    1024 * 6, 
                                                    g_wifi_cmd_queue, 
                                                    3, 
                                                    &s_wifi_task_handle, 
                                                    0);
        if(ret != pdPASS)
        {
            ESP_LOGE(TAG, "Failed to create wifi task");
            s_wifi_task_handle = NULL;
        }
    }
}

/* --------------------------- END WIFI任务实现 ---------------------------*/

/* ------------------------ 时间任务实现（core 0） ------------------------ */

/**
 * @brief 时间任务函数
 * @param pvParameters 无
 */
static void datetime_task(void *pvParameters)
{
    (void)pvParameters;

    TickType_t last_wake_time = xTaskGetTickCount();
    while (1) 
    {
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
        update_home_time_display();
    }
}

/**
 * @brief 创建时间任务
 */
static void create_datetime_task(void) 
{
    if (s_time_task_handle == NULL) 
    {
        BaseType_t ret = xTaskCreatePinnedToCoreWithCaps(datetime_task, 
                                                        "time_task", 
                                                        1024 * 3, 
                                                        NULL, 
                                                        2, 
                                                        &s_time_task_handle, 
                                                        0, 
                                                        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (ret != pdPASS) 
        {
            ESP_LOGE(TAG, "Failed to create time task");
            s_time_task_handle = NULL;
        }
    }
}

/* --------------------------- END 时间任务实现 ---------------------------*/

/* -------------------- 天气任务实现（core 0） -------------------- */

/**
 * @brief 天气任务函数
 * @param pvParameters 无
 */
static void weather_task(void *pvParameters) 
{
    (void)pvParameters;
    // 等待 WIFI 连接
    while(!wifi_connect_state())
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    get_weather();
    
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1) 
    {
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(WEATHER_REFRESH_INTERVAL_MS));
        get_weather();
    }
}

/**
 * @brief 创建天气任务
 */
static void create_weather_task(void) 
{
    if (s_weather_task_handle == NULL) 
    {
        BaseType_t ret = xTaskCreatePinnedToCoreWithCaps(weather_task, 
                                                        "weather_task", 
                                                        1024 * 5, 
                                                        NULL, 
                                                        2, 
                                                        &s_weather_task_handle, 
                                                        0, 
                                                        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (ret != pdPASS) 
        {
            ESP_LOGE(TAG, "Failed to create weather task");
            s_weather_task_handle = NULL;
        }
    }
}

/* --------------------------- END 天气任务实现 ---------------------------*/

/* ---------------------- 串口接收任务实现（core 0） ----------------------- */

/**
 * @brief 串口接收任务函数
 * @param pvParameters 未使用
 */
static void serial_task(void *pvParameters)
{
    (void)pvParameters;
    uint8_t buffer[SERIAL_RX_CHUNK_SIZE];
    int len;

    while (1) {
        if (serial_get_rx_active()) {
            // 阻塞等待数据，超时 100ms 以便检查 active 标志
            len = serial_port_read(buffer, SERIAL_RX_CHUNK_SIZE, 0);
            if (len > 0) {
                // 调用 UI 更新函数（内部会加锁）
                update_serial_display((const char *)buffer, len);
            }
            else{
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(100)); // 未激活时低功耗等待
        }
    }
}

/**
 * @brief 启动串口接收任务（由 app_task_init 调用一次）
 */
static void create_serial_task(void)
{
    if (s_serial_task_handle == NULL) {
        BaseType_t ret = xTaskCreatePinnedToCoreWithCaps(serial_task, 
                                                        "serial_task", 
                                                        1024 * 5, 
                                                        NULL, 
                                                        2, 
                                                        &s_serial_task_handle, 
                                                        0, 
                                                        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT); // core 0 运行
        if (ret != pdPASS) {
            ESP_LOGE(TAG, "Failed to create serial task");
            s_serial_task_handle = NULL;
        }
    }
}

/* --------------------------- END 串口接收任务实现 ---------------------------*/

/* ------------------------ 音乐任务实现（core 0） ------------------------- */

/**
 * @brief 音乐任务函数
 * @param pvParameters 未使用
 */
static void music_task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        /* 等待通知，或播放完成 */
        if (!music_is_playing()) {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        }
        music_play();
        vTaskDelay(1);
    }
}

/**
 * @brief 创建音乐任务
 */
static void create_music_task(void)
{
    if (s_music_task_handle == NULL)
    {
        /* 先初始化音乐 BSP（I2S/解码器） */
        if (music_init() != ESP_OK)
        {
            ESP_LOGE(TAG, "Music init failed, task not created");
            return;
        }

        BaseType_t ret = xTaskCreatePinnedToCoreWithCaps(music_task, 
                                                        "music_task", 
                                                        1024 * 20,
                                                        NULL, 
                                                        1, 
                                                        &s_music_task_handle, 
                                                        0, 
                                                        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (ret != pdPASS)
        {
            ESP_LOGE(TAG, "Failed to create music task");
            s_music_task_handle = NULL;
        }
    }
}

/**
 * @brief 启动音乐播放循环
 */
void music_task_start(void)
{
    if (s_music_task_handle != NULL) {
        xTaskNotifyGive(s_music_task_handle);
    }
}

/**
 * @brief 通知共享播放器停止，任务空闲后等待下一条命令
 */
void music_task_stop(void) 
{
    music_cmd_msg_t msg = { .cmd = MUSIC_CMD_STOP };
    music_send_cmd(&msg);
}


/* --------------------------- END 音乐任务实现 ---------------------------*/

/* --------------------------- AI任务实现 ---------------------------*/

static void create_ai_task(void)
{
    if (s_ai_task_handle != NULL)
        return;

    if (xfyun_ai_init() != ESP_OK) {
        ESP_LOGE(TAG, "AI init failed");
        return;
    }

    BaseType_t ret = xTaskCreatePinnedToCoreWithCaps(xfyun_ai_task, 
                                                        "ai_task", 
                                                        1024 * 16,
                                                        NULL, 
                                                        2, 
                                                        &s_ai_task_handle, 
                                                        0, 
                                                        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create AI task");
        s_ai_task_handle = NULL;
    }
}

/* --------------------------- END AI任务实现 ---------------------------*/



/* --------------------------- OneNET task --------------------------- */

static void onenet_task(void *pvParameters)
{
    (void)pvParameters;

    while (1) {
        while (!wifi_connect_state()) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        esp_err_t ret = onenet_start();
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "OneNET MQTT client started, waiting for broker connection");
            break;
        }

        ESP_LOGE(TAG, "OneNET MQTT start failed: %s, retrying...",
                 esp_err_to_name(ret));
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    s_onenet_task_handle = NULL;
    vTaskDelete(NULL);
}

static void create_onenet_task(void)
{
    if (s_onenet_task_handle != NULL) {
        return;
    }

    BaseType_t ret = xTaskCreatePinnedToCore(onenet_task, "onenet_task",
                                             4096, NULL, 2,
                                             &s_onenet_task_handle, 0);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create OneNET task");
        s_onenet_task_handle = NULL;
    }
}

/* --------------------------- END OneNET task --------------------------- */

/* ----------------------- task_init函数，外部调用 ----------------------- */

/**
 * @brief 初始化任务
 */
esp_err_t mic_tasks_create(TaskHandle_t *writer, TaskHandle_t *capture)
{
    if (xTaskCreatePinnedToCoreWithCaps(mic_writer_task, "mic_writer", 6144,
                                       NULL, 3, writer, 0,
                                       MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create mic_writer task");
        return ESP_ERR_NO_MEM;
    }
    if (xTaskCreatePinnedToCoreWithCaps(mic_capture_task, "mic_capture", 4096,
                                       NULL, 4, capture, 0,
                                       MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create mic_capture task");
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

void app_task_init(void)
{
    bind_wifi_status_event_handler(&guider_ui);
    bind_music_event_handler(&guider_ui);
    create_wifi_task();
    create_onenet_task();
    create_datetime_task();
    create_weather_task();
    create_serial_task();
    create_music_task();
    create_ai_task();
    esp_err_t mic_err = mic_init();
    if (mic_err != ESP_OK) {
        ESP_LOGE(TAG, "Microphone init failed: %s (0x%x)",
                 esp_err_to_name(mic_err), (unsigned int)mic_err);
    }
}

/* ---------------------------- END ----------------------------- */

