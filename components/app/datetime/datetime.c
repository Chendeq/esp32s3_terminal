#include "datetime.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sntp.h"
#include "time.h"
#include "esp_err.h"

static const char *TAG = "datetime";

static bool s_sntp_initialized = false;         // SNTP初始化标志
static bool s_time_synced = false;              // 时间同步标志

// 星期几的字符串表示
static const char *week_days[] = {"星期日", "星期一", "星期二", "星期三", 
                                   "星期四", "星期五", "星期六"};

/**
 * @brief 初始化SNTP
 */
static void esp_initialize_sntp(void)
{    
    ESP_LOGI(TAG, "Initializing SNTP");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);    // 设置为轮询模式
    esp_sntp_setservername(0, "pool.ntp.org");       // 设置服务器0为pool.ntp.org
    esp_sntp_setservername(1, "cn.pool.ntp.org");    // 设置服务器1为cn.pool.ntp.org
    esp_sntp_setservername(2, "ntp1.aliyun.com");    // 设置服务器2为ntp1.aliyun.com
    esp_sntp_init();                                  // 初始化SNTP
    s_sntp_initialized = true;
}

/**
 * @brief 等待SNTP时间同步
 */
void datetime_wait_sntp_sync(void)
{
    // 如果时间已同步，直接返回
    if (s_time_synced)
        return;

    // 如果SNTP未初始化，先初始化
    if (!s_sntp_initialized) 
    {
        ESP_LOGI(TAG, "Waiting for SNTP time sync...");
        esp_initialize_sntp();
    }

    struct tm timeinfo; // 时间信息结构体
    time_t now = 0;     // 当前时间戳
    int retry = 0;      // 重试次数
    int max_retries = 50; // 最大重试次数

    timeinfo.tm_year = 0; // 初始化年份为0
    
    // 等待系统时间被设置
    while (timeinfo.tm_year < (2024 - 1900) && retry < max_retries) 
    {
        ESP_LOGD(TAG, "Waiting for system time to be set... (%d/%d)", ++retry, max_retries);
        vTaskDelay(pdMS_TO_TICKS(100));
        time(&now); // 获取当前时间戳   
        localtime_r(&now, &timeinfo); // 将时间戳转换为本地时间结构体
    }

    if (timeinfo.tm_year >= (2024 - 1900)) // 如果年份大于等于2024年
    {
        setenv("TZ", "CST-8", 1);   // 设置时区为东八区
        tzset();                     // 设置系统时区
        s_time_synced = true;        // 标记时间已同步
        ESP_LOGI(TAG, "SNTP time synchronized");
    } 
    else 
    {
        ESP_LOGW(TAG, "SNTP sync failed after %d retries", max_retries);
    }
}

/**
 * @brief 检查时间是否同步
 * @return true 时间已同步
 * @return false 时间未同步
 */
bool datetime_is_time_synced(void)
{
    return s_time_synced;
}

/**
 * @brief 获取当前时间字符串
 * @param time_buf 时间字符串缓冲区
 * @param time_len 时间字符串缓冲区长度
 * @param date_buf 日期字符串缓冲区
 * @param date_len 日期字符串缓冲区长度
 * @param week_buf 星期几字符串缓冲区
 * @param week_len 星期几字符串缓冲区长度
 */
void datetime_get_current_time_str(char *time_buf, size_t time_len, char *date_buf, size_t date_len, char *week_buf, size_t week_len)
{
    time_t now = 0;     // 当前时间戳
    struct tm timeinfo; // 时间信息结构体
    time(&now);         // 获取当前时间戳   
    localtime_r(&now, &timeinfo); // 将时间戳转换为本地时间结构体
    
    // 如果时间未同步，返回默认值
    if (timeinfo.tm_year < (2024 - 1900)) {
        snprintf(time_buf, time_len, "--:--:--");
        snprintf(date_buf, date_len, "----/--/--");
        snprintf(week_buf, week_len, "---");
        return;
    }
    
    // 格式化时间字符串，格式为 HH:MM:SS
    snprintf(time_buf, time_len, "%02d:%02d:%02d", 
             timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    
    int wday = timeinfo.tm_wday; // 获取星期几
    const char *weekday_str = (wday >= 0 && wday <= 6) ? week_days[wday] : "未知";
    snprintf(week_buf, week_len, "%s", weekday_str);
    
    // 格式化日期字符串，格式为 YYYY/MM/DD
    snprintf(date_buf, date_len, "%d/%02d/%02d", 
             timeinfo.tm_year + 1900, // 年
             timeinfo.tm_mon + 1,     // 月
             timeinfo.tm_mday);
}

/**
 * @brief 获取当前日期,更新calendar显示日期
 * @param year 年指针
 * @param month 月指针
 * @param day 日指针
 */
void datetime_get_current_date(int *year, int *month, int *day)
{
    time_t now = 0;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    
    if (year) 
        *year = timeinfo.tm_year + 1900;
    if (month) 
        *month = timeinfo.tm_mon + 1;
    if (day) 
        *day = timeinfo.tm_mday;
}
