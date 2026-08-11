#include "weather.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_http_client.h"
#include "esp_err.h"
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"
#include "url_encode.h"
#include "lwip/netdb.h"

static const char *TAG = "weather";

#define WEATHER_PRIVATE_KEY "S1rY4Z32dKSlhZ2DD"     // 心知天气 API 密钥

// 缓冲区定义
#define MAX_OUTPUT_BUFFER_LEN   1536
static char output_buffer[MAX_OUTPUT_BUFFER_LEN];
static int output_len = 0;

// 复用 JSON 缓冲区
static char now_json[MAX_OUTPUT_BUFFER_LEN];
static char daily_json[MAX_OUTPUT_BUFFER_LEN];

// 城市名称缓冲区
static char s_city_name[64] = "";

static weather_cache_t s_weather_cache = { .has_data = false };

// HTTP事件组
#define CLIENT_HTTP_RECEIVE_BIT BIT0
static EventGroupHandle_t s_client_http_event = NULL;

static esp_err_t parse_location_data(char* location_js);
static bool http_get(const char *url, char *buffer, size_t max_len);
static void copy_str_trunc(char *dst, size_t dst_size, const char *src);

static void copy_str_trunc(char *dst, size_t dst_size, const char *src)
{
    if (!dst || dst_size == 0) {
        return;
    }

    if (!src) {
        dst[0] = '\0';
        return;
    }

    snprintf(dst, dst_size, "%s", src);
}

/* ==================== HTTP 事件回调 ==================== */
static esp_err_t http_client_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) 
    {
        case HTTP_EVENT_ERROR:
            break;
        case HTTP_EVENT_ON_CONNECTED:
            break;
        case HTTP_EVENT_HEADER_SENT:
            break;
        case HTTP_EVENT_ON_HEADER:
            break;
        case HTTP_EVENT_ON_DATA:
            if (s_client_http_event) 
                xEventGroupClearBits(s_client_http_event, CLIENT_HTTP_RECEIVE_BIT);
            if(output_len + evt->data_len < MAX_OUTPUT_BUFFER_LEN)
            {
                memcpy(&output_buffer[output_len], evt->data, evt->data_len);
                output_len += evt->data_len;
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            if (s_client_http_event)
                xEventGroupSetBits(s_client_http_event, CLIENT_HTTP_RECEIVE_BIT);
            break;
        case HTTP_EVENT_DISCONNECTED:
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            break;
    }
    return ESP_OK;
}

static bool http_get(const char *url, char *buffer, size_t max_len)
{
    if (s_client_http_event == NULL)
        s_client_http_event = xEventGroupCreate();

    output_len = 0;
    memset(output_buffer, 0, sizeof(output_buffer));

    // 初始化 HTTP 客户端配置
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = http_client_event_handler,
        .timeout_ms = 10000,
    };

    // 初始化 HTTP 客户端
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) return false;

    // 执行 HTTP GET 请求
    esp_err_t err = esp_http_client_perform(client);
    bool ok = false;
    if (err == ESP_OK && esp_http_client_get_status_code(client) == 200) // 
    {
        int read_len;
        do {
            read_len = esp_http_client_read(client, output_buffer + output_len,
                                            sizeof(output_buffer) - output_len - 1);
            if (read_len > 0) output_len += read_len;
        } while (read_len > 0);
        output_buffer[output_len] = '\0';
        strncpy(buffer, output_buffer, max_len - 1);
        buffer[max_len - 1] = '\0';
        ok = true;
    }
    esp_http_client_cleanup(client);
    return ok;
}

/* ========= 解析位置，更新 city 缓存 ========= */
static esp_err_t parse_location_data(char *location_js)
{
    cJSON *root = cJSON_Parse(location_js);
    if(!root) 
    {
        ESP_LOGE(TAG, "Invalid json format");
        return ESP_FAIL;
    }

    // 检查 ret 字段是否为 "ok"，否则直接返回
    cJSON *ret = cJSON_GetObjectItem(root, "ret"); 
    if (!ret || !cJSON_IsString(ret) || strcmp(ret->valuestring, "ok") != 0) 
    {
        ESP_LOGE(TAG, "API returned error status: %s", ret ? ret->valuestring : "unknown");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    // 获取 data 对象，包含 IP 和 location 数组
    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (!data) 
    {
        ESP_LOGE(TAG, "No data field in response");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    // 获取 location 数组
    cJSON *location = cJSON_GetObjectItem(data, "location");
    if (!location || !cJSON_IsArray(location)) 
    {
        ESP_LOGE(TAG, "No location array");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    int array_size = cJSON_GetArraySize(location);  // 获取数组大小
    if (array_size < 2) 
    {
        ESP_LOGE(TAG, "location array too small");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    // location 数组结构: ["国家","省份","城市","","运营商"]
    // 取城市（第三个元素），如果为空则取省份
    cJSON *city_item = cJSON_GetArrayItem(location, 2);
    if (!city_item || !cJSON_IsString(city_item) || strlen(city_item->valuestring) == 0) 
    {
        city_item = cJSON_GetArrayItem(location, 1);
    }
    if (!city_item || !cJSON_IsString(city_item)) 
    {
        ESP_LOGE(TAG, "Failed to extract city/province");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    const char *city_name = cJSON_GetStringValue(city_item);
    snprintf(s_city_name, sizeof(s_city_name), "%s", city_name);
    copy_str_trunc(s_weather_cache.city, sizeof(s_weather_cache.city), s_city_name);
    ESP_LOGI(TAG, "Current city: %s", s_city_name);
    cJSON_Delete(root);
    return ESP_OK;
}

/* ========= 解析实时天气，更新缓存 ========= */
static void parse_now_to_cache(char *json)
{
    cJSON *root = cJSON_Parse(json);
    if (!root) return;
    cJSON *results = cJSON_GetObjectItem(root, "results");
    if (!results || !cJSON_IsArray(results)) {
        cJSON_Delete(root);
        return;
    }
    cJSON *first = cJSON_GetArrayItem(results, 0);
    if (!first) {
        cJSON_Delete(root);
        return;
    }

    // 解析实时天气
    cJSON *now = cJSON_GetObjectItem(first, "now");
    if (now) {
        cJSON *temp = cJSON_GetObjectItem(now, "temperature");
        cJSON *text = cJSON_GetObjectItem(now, "text");
        if (temp && text) {
            copy_str_trunc(s_weather_cache.temp, sizeof(s_weather_cache.temp), cJSON_GetStringValue(temp));
            copy_str_trunc(s_weather_cache.weather, sizeof(s_weather_cache.weather), cJSON_GetStringValue(text));
        }
    }

    // 解析最后更新时间
    cJSON *last_update = cJSON_GetObjectItem(first, "last_update");
    if (last_update && cJSON_IsString(last_update)) {
        copy_str_trunc(s_weather_cache.last_update, sizeof(s_weather_cache.last_update), cJSON_GetStringValue(last_update));
    } else {
        s_weather_cache.last_update[0] = '\0';
    }
    cJSON_Delete(root);
}

/* ========= 解析未来三天天气，更新缓存 ========= */
static void parse_daily_to_cache(char *json)
{
    cJSON *root = cJSON_Parse(json);
    if (!root) return;
    cJSON *results = cJSON_GetObjectItem(root, "results");
    if (!results || !cJSON_IsArray(results)) {
        cJSON_Delete(root);
        return;
    }
    cJSON *first = cJSON_GetArrayItem(results, 0);
    if (!first) {
        cJSON_Delete(root);
        return;
    }
    cJSON *daily = cJSON_GetObjectItem(first, "daily");
    if (daily && cJSON_IsArray(daily)) {
        int count = cJSON_GetArraySize(daily);
        if (count > 3) count = 3;
        for (int i = 0; i < count; i++) {
            cJSON *day = cJSON_GetArrayItem(daily, i);
            if (!day) continue;
            cJSON *date = cJSON_GetObjectItem(day, "date");
            cJSON *text = cJSON_GetObjectItem(day, "text_day");
            cJSON *high = cJSON_GetObjectItem(day, "high");
            cJSON *low = cJSON_GetObjectItem(day, "low");
            if (date && text && high && low) {
                const char *full_date = cJSON_GetStringValue(date);
                char month_day[8] = {0};
                int year, month, day;
                if (sscanf(full_date, "%d-%d-%d", &year, &month, &day) == 3) {
                    snprintf(month_day, sizeof(month_day), "%02d-%02d", month, day);
                } else {
                    copy_str_trunc(month_day, sizeof(month_day), full_date);
                }
                copy_str_trunc(s_weather_cache.forecast_date[i], sizeof(s_weather_cache.forecast_date[i]), month_day);
                copy_str_trunc(s_weather_cache.forecast_weather[i], sizeof(s_weather_cache.forecast_weather[i]), cJSON_GetStringValue(text));
                snprintf(s_weather_cache.forecast_temp[i], sizeof(s_weather_cache.forecast_temp[0]), "%s~%s℃", cJSON_GetStringValue(low), cJSON_GetStringValue(high));
            }
        }
    }
    cJSON_Delete(root);
}

/* ========= 外部 API：获取位置 ========= */
esp_err_t get_location(void)
{
    // 等待 DNS
    int dns_retry = 0;
    while (dns_retry < 20) {
        struct hostent *host = gethostbyname("myip.ipip.net");
        if (host != NULL) break;
        vTaskDelay(pdMS_TO_TICKS(100));
        dns_retry++;
    }
    char location_json[512] = {0};
    if (!http_get("https://myip.ipip.net/json", location_json, sizeof(location_json))) {
        ESP_LOGW(TAG, "Failed to get location, use default: 贵港");
        strcpy(s_city_name, "贵港");
        copy_str_trunc(s_weather_cache.city, sizeof(s_weather_cache.city), s_city_name);
        return ESP_FAIL;
    }
    return parse_location_data(location_json);
}

/* ========= 外部 API：获取天气（实时+未来），更新缓存 ========= */
esp_err_t get_weather(void)
{
    if (strlen(s_city_name) == 0 || !s_weather_cache.has_data) {
        get_location();
    }
    char *encoded_city = url_encode_alloc(s_city_name, -1);     // 编码城市名称
    if (!encoded_city) {
        encoded_city = s_city_name;  // 对城市名称编码失败，则使用默认城市
    }

    // 实时天气
    char now_url[512];
    snprintf(now_url, sizeof(now_url),
             "https://api.seniverse.com/v3/weather/now.json?key=%s&location=%s&language=zh-Hans&unit=c",
             WEATHER_PRIVATE_KEY, encoded_city);
    if (http_get(now_url, now_json, sizeof(now_json))) {
        parse_now_to_cache(now_json);
    } else {
        ESP_LOGE(TAG, "Failed to get current weather");
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    // 未来三天天气
    char daily_url[512];
    snprintf(daily_url, sizeof(daily_url),
             "https://api.seniverse.com/v3/weather/daily.json?key=%s&location=%s&language=zh-Hans&unit=c&start=0&days=3",
             WEATHER_PRIVATE_KEY, encoded_city);
    if (http_get(daily_url, daily_json, sizeof(daily_json))) {
        parse_daily_to_cache(daily_json);
    } else {
        ESP_LOGE(TAG, "Failed to get daily forecast");
    }
    // 释放编码后的城市名称内存
    if (encoded_city != s_city_name) {
        free(encoded_city);
    }

    s_weather_cache.has_data = true;
    return ESP_OK;
}

/* ========= 获取缓存的天气数据 ========= */
void weather_get_cached(weather_cache_t *out)
{
    if (out) {
        memcpy(out, &s_weather_cache, sizeof(weather_cache_t));
    }
}
