#include "onenet_ota.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "esp_app_desc.h"
#include "esp_heap_caps.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"

#include "datetime.h"
#include "onenet_mqtt.h"
#include "onenet_token.h"

#define TAG "onenet_ota"

#define OTA_API_BASE_URL "https://iot-api.heclouds.com/fuse-ota"
#define OTA_HTTP_TIMEOUT_MS 15000
#define OTA_DOWNLOAD_TIMEOUT_MS 30000
#define OTA_RESPONSE_MAX_SIZE 2048
#define OTA_VERSION_MAX_SIZE 32
#define OTA_URL_MAX_SIZE 320
#define OTA_WORKER_STACK_SIZE 8192
#define OTA_WORKER_PRIORITY 3
#define OTA_ROLLBACK_STACK_SIZE 4096
#define OTA_ROLLBACK_PRIORITY 2

/*
 * OneNET HTTPS 根证书：DigiCert Global Root G2
 *
 * 证书链：
 * *.heclouds.com
 *   -> GeoTrust G2 TLS CN RSA4096 SHA256 2022 CA1
 *   -> DigiCert Global Root G2
 *
 * 显式设置 cert_pem 后，即使全局配置允许跳过证书校验，OTA 连接仍会
 * 强制校验证书链、主机名和有效期。
 * 该根证书有效期至 2038-01-15，到期前需要更新。
 */
static const char ONENET_OTA_CA_PEM[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
    "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n"
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n"
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n"
    "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n"
    "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n"
    "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n"
    "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n"
    "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n"
    "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n"
    "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n"
    "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n"
    "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n"
    "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n"
    "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n"
    "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n"
    "MrY=\n"
    "-----END CERTIFICATE-----\n";

/* HTTP 响应接收上下文 */
typedef struct
{
    char *buffer;
    size_t capacity;
    size_t length;
    bool overflow;
} ota_response_t;

/* OneNET 升级任务信息 */
typedef struct
{
    int id;
    int type;
    char target[OTA_VERSION_MAX_SIZE];
} ota_task_t;

static bool s_initialized;
static volatile bool s_running;
static TaskHandle_t s_worker_handle;
static TaskHandle_t s_rollback_handle;
static ota_task_t s_task;

static void ota_worker(void *argument);

/* 获取当前运行固件版本 */
static const char *running_version(void)
{
    static char version[OTA_VERSION_MAX_SIZE];
    const esp_app_desc_t *description = esp_app_get_description();

    if (version[0] == '\0')
    {
        snprintf(version, sizeof(version), "%s",
                 description != NULL ? description->version : "unknown");
    }
    return version;
}

/* 生成 OneNET API Token */
static esp_err_t api_token(char *token, size_t size)
{
    return dev_token_generate_ex(token, size, SIG_METHOD_SHA256,
                                 ONENET_TOKEN_EXPIRE_TIME, ONENET_PRODUCT_ID,
                                 NULL, ONENET_ACCESS_KEY) == 0
               ? ESP_OK
               : ESP_FAIL;
}

/* 累积 HTTP 响应数据 */
static esp_err_t on_http_data(esp_http_client_event_t *event)
{
    ota_response_t *response = event->user_data;
    size_t length;

    if (event->event_id != HTTP_EVENT_ON_DATA || event->data_len <= 0)
    {
        return ESP_OK;
    }

    length = (size_t)event->data_len;
    if (response == NULL || response->buffer == NULL ||
        response->length + length >= response->capacity)
    {
        if (response != NULL)
        {
            response->overflow = true;
        }
        return ESP_OK;
    }

    memcpy(response->buffer + response->length, event->data, length);
    response->length += length;
    response->buffer[response->length] = '\0';
    return ESP_OK;
}

/* 调用 OneNET OTA API，并校验服务器证书 */
static esp_err_t api_request(const char *url, esp_http_client_method_t method,
                             const char *body, char *response,
                             size_t response_size)
{
    char token[ONENET_TOKEN_MAX_SIZE];
    ota_response_t response_ctx = {
        .buffer = response,
        .capacity = response_size,
    };
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = on_http_data,
        .user_data = &response_ctx,
        .cert_pem = ONENET_OTA_CA_PEM,
        .timeout_ms = OTA_HTTP_TIMEOUT_MS,
        .keep_alive_enable = false,
    };
    esp_http_client_handle_t client;
    esp_err_t result;
    int status;

    if (url == NULL || response == NULL || response_size == 0)
    {
        return ESP_ERR_INVALID_ARG;
    }
    response[0] = '\0';

    result = api_token(token, sizeof(token));
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "生成 OneNET Token 失败");
        return result;
    }

    client = esp_http_client_init(&config);
    if (client == NULL)
    {
        return ESP_ERR_NO_MEM;
    }

    result = esp_http_client_set_method(client, method);
    if (result == ESP_OK)
    {
        result = esp_http_client_set_header(client, "Content-Type",
                                            "application/json");
    }
    if (result == ESP_OK)
    {
        result = esp_http_client_set_header(client, "Authorization", token);
    }
    if (result == ESP_OK && body != NULL)
    {
        result = esp_http_client_set_post_field(client, body, (int)strlen(body));
    }
    if (result == ESP_OK)
    {
        result = esp_http_client_perform(client);
    }

    status = esp_http_client_get_status_code(client);
    if (result == ESP_OK && (status < 200 || status >= 300))
    {
        ESP_LOGE(TAG, "OneNET API HTTP 状态码异常：%d", status);
        result = ESP_FAIL;
    }
    if (response_ctx.overflow)
    {
        ESP_LOGE(TAG, "OneNET API 响应超过 %u 字节",
                 (unsigned int)response_size);
        result = ESP_ERR_INVALID_SIZE;
    }
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "OneNET API 请求失败：%s，内部内存=%u，最大块=%u，PSRAM=%u",
                 esp_err_to_name(result),
                 (unsigned int)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                 (unsigned int)heap_caps_get_largest_free_block(
                     MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),
                 (unsigned int)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    }

    esp_http_client_cleanup(client);
    return result;
}

/* 判断是否为可重试的临时网络错误 */
static bool retryable_network_error(esp_err_t error)
{
    return error == ESP_ERR_HTTP_CONNECT ||
           error == ESP_ERR_HTTP_CONNECTING ||
           error == ESP_ERR_HTTP_CONNECTION_CLOSED ||
           error == ESP_ERR_HTTP_READ_TIMEOUT ||
           error == ESP_ERR_HTTP_WRITE_DATA ||
           error == ESP_ERR_HTTP_INCOMPLETE_DATA;
}

/* 网络瞬时断连时重试，避免一次 TCP 重置终止 OTA */
static esp_err_t api_request_retry(const char *url,
                                   esp_http_client_method_t method,
                                   const char *body, char *response,
                                   size_t response_size)
{
    esp_err_t result = ESP_FAIL;

    for (int attempt = 0; attempt < 3; ++attempt) {
        result = api_request(url, method, body, response, response_size);
        if (result == ESP_OK || !retryable_network_error(result)) {
            break;
        }
        if (attempt < 2) {
            ESP_LOGW(TAG, "OneNET API 连接中断，1 秒后重试（%d/3）",
                     attempt + 2);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    return result;
}

/* 判断 OneNET 响应中的 code 是否为 0 */
static bool response_ok(cJSON *root)
{
    cJSON *code = cJSON_GetObjectItemCaseSensitive(root, "code");
    return cJSON_IsNumber(code) && code->valueint == 0;
}

/* 输出 OneNET API 返回的错误信息 */
static void log_response_error(const char *action, cJSON *root)
{
    cJSON *code = cJSON_GetObjectItemCaseSensitive(root, "code");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(root, "msg");

    ESP_LOGE(TAG, "%s失败：code=%d，msg=%s", action,
             cJSON_IsNumber(code) ? code->valueint : -1,
             cJSON_IsString(message) ? message->valuestring : "unknown");
}

/* 上报当前固件版本 */
static esp_err_t upload_version(void)
{
    char url[OTA_URL_MAX_SIZE];
    char response[OTA_RESPONSE_MAX_SIZE];
    char body[128];
    const char *version = running_version();
    cJSON *root;
    esp_err_t result;

    snprintf(url, sizeof(url), "%s/%s/%s/version", OTA_API_BASE_URL,
             ONENET_PRODUCT_ID, ONENET_DEVICE_NAME);
    snprintf(body, sizeof(body),
             "{\"s_version\":\"%s\",\"f_version\":\"%s\"}", version, version);
    result = api_request_retry(url, HTTP_METHOD_POST, body, response,
                               sizeof(response));
    if (result != ESP_OK)
    {
        return result;
    }

    root = cJSON_Parse(response);
    if (root == NULL)
    {
        ESP_LOGE(TAG, "版本上报响应格式错误：%s", response);
        return ESP_FAIL;
    }
    result = response_ok(root) ? ESP_OK : ESP_FAIL;
    if (result != ESP_OK)
    {
        log_response_error("版本上报", root);
    }
    cJSON_Delete(root);

    if (result == ESP_OK)
    {
        ESP_LOGI(TAG, "版本上报成功：%s", version);
    }
    return result;
}

/* 对版本号进行 URL 编码，避免特殊字符破坏查询参数 */
static bool url_encode_version(const char *input, char *output, size_t size)
{
    static const char hex[] = "0123456789ABCDEF";
    size_t offset = 0;

    for (const unsigned char *p = (const unsigned char *)input; *p; ++p)
    {
        bool plain = (*p >= 'A' && *p <= 'Z') ||
                     (*p >= 'a' && *p <= 'z') ||
                     (*p >= '0' && *p <= '9') ||
                     *p == '-' || *p == '.' || *p == '_' || *p == '~';
        size_t need = plain ? 1 : 3;

        if (offset + need >= size)
        {
            return false;
        }
        if (plain)
        {
            output[offset++] = (char)*p;
        }
        else
        {
            output[offset++] = '%';
            output[offset++] = hex[*p >> 4];
            output[offset++] = hex[*p & 0x0F];
        }
    }

    output[offset] = '\0';
    return true;
}

/* 查询指定类型的 OTA 任务；type=2 为 MCU，type=1 为升级模块 */
static esp_err_t check_task(const char *type, const char *version)
{
    char encoded[96];
    char url[OTA_URL_MAX_SIZE];
    char response[OTA_RESPONSE_MAX_SIZE];
    cJSON *root;
    cJSON *data;
    cJSON *target;
    cJSON *task_id;
    esp_err_t result;

    if (!url_encode_version(version, encoded, sizeof(encoded)))
    {
        return ESP_ERR_INVALID_ARG;
    }
    snprintf(url, sizeof(url), "%s/%s/%s/check?type=%s&version=%s",
             OTA_API_BASE_URL, ONENET_PRODUCT_ID, ONENET_DEVICE_NAME, type,
             encoded);

    result = api_request_retry(url, HTTP_METHOD_GET, NULL, response,
                               sizeof(response));
    if (result != ESP_OK)
    {
        return result;
    }

    root = cJSON_Parse(response);
    if (root == NULL)
    {
        ESP_LOGE(TAG, "OTA 查询响应格式错误：%s", response);
        return ESP_FAIL;
    }

    if (!response_ok(root))
    {
        cJSON *code = cJSON_GetObjectItemCaseSensitive(root, "code");
        cJSON *message = cJSON_GetObjectItemCaseSensitive(root, "msg");
        ESP_LOGI(TAG, "OTA 查询 type=%s：code=%d，msg=%s", type,
                 cJSON_IsNumber(code) ? code->valueint : -1,
                 cJSON_IsString(message) ? message->valuestring : "unknown");
        cJSON_Delete(root);
        return ESP_ERR_NOT_FOUND;
    }

    data = cJSON_GetObjectItemCaseSensitive(root, "data");
    if (!cJSON_IsObject(data))
    {
        cJSON_Delete(root);
        return ESP_ERR_NOT_FOUND;
    }

    target = cJSON_GetObjectItemCaseSensitive(data, "target");
    task_id = cJSON_GetObjectItemCaseSensitive(data, "tid");
    if (!cJSON_IsString(target) || !cJSON_IsNumber(task_id) ||
        task_id->valueint <= 0)
    {
        cJSON_Delete(root);
        return ESP_ERR_NOT_FOUND;
    }

    s_task.id = task_id->valueint;
    s_task.type = atoi(type);
    snprintf(s_task.target, sizeof(s_task.target), "%s", target->valuestring);
    cJSON_Delete(root);

    ESP_LOGI(TAG, "发现 OTA 任务：id=%d，type=%d，目标版本=%s",
             s_task.id, s_task.type, s_task.target);
    return ESP_OK;
}

/* 上报 OTA 升级进度 */
static esp_err_t upload_status(int task_id, int step)
{
    char url[OTA_URL_MAX_SIZE];
    char response[OTA_RESPONSE_MAX_SIZE];
    char body[32];
    cJSON *root;
    esp_err_t result;

    snprintf(url, sizeof(url), "%s/%s/%s/%d/status", OTA_API_BASE_URL,
             ONENET_PRODUCT_ID, ONENET_DEVICE_NAME, task_id);
    snprintf(body, sizeof(body), "{\"step\":%d}", step);

    result = api_request_retry(url, HTTP_METHOD_POST, body, response,
                               sizeof(response));
    if (result != ESP_OK)
    {
        return result;
    }

    root = cJSON_Parse(response);
    if (root == NULL)
    {
        return ESP_FAIL;
    }
    result = response_ok(root) ? ESP_OK : ESP_FAIL;
    if (result != ESP_OK)
    {
        log_response_error("升级进度上报", root);
    }
    cJSON_Delete(root);
    return result;
}

/* 下载升级包前的请求头设置 */
static esp_err_t download_client_init(esp_http_client_handle_t client)
{
    char token[ONENET_TOKEN_MAX_SIZE];
    esp_err_t result = api_token(token, sizeof(token));

    if (result == ESP_OK)
    {
        result = esp_http_client_set_method(client, HTTP_METHOD_GET);
    }
    if (result == ESP_OK)
    {
        result = esp_http_client_set_header(client, "Content-Type",
                                            "application/json");
    }
    if (result == ESP_OK)
    {
        result = esp_http_client_set_header(client, "Authorization", token);
    }
    return result;
}

/* 下载固件；esp_https_ota 会同时校验镜像格式、SHA-256 并写入分区 */
static esp_err_t download_image(int task_id)
{
    char url[OTA_URL_MAX_SIZE];
    esp_http_client_config_t http_config = {
        .url = url,
        .cert_pem = ONENET_OTA_CA_PEM,
        .timeout_ms = OTA_DOWNLOAD_TIMEOUT_MS,
        .keep_alive_enable = false,
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
        .http_client_init_cb = download_client_init,
    };
    esp_err_t result;

    snprintf(url, sizeof(url), "%s/%s/%s/%d/download", OTA_API_BASE_URL,
             ONENET_PRODUCT_ID, ONENET_DEVICE_NAME, task_id);

    for (int attempt = 0; attempt < 3; ++attempt) {
        result = esp_https_ota(&ota_config);
        if (result == ESP_OK) {
            ESP_LOGI(TAG, "固件下载及写入成功");
            break;
        }
        if (!retryable_network_error(result)) {
            break;
        }
        if (attempt < 2) {
            ESP_LOGW(TAG, "固件下载连接中断，1 秒后重试（%d/3）",
                     attempt + 2);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "固件下载失败：%s", esp_err_to_name(result));
    }
    return result;
}

/* 新固件自检超时后回滚 */
static void rollback_watchdog(void *argument)
{
    (void)argument;

    if (ulTaskNotifyTake(pdTRUE,
                         pdMS_TO_TICKS(ONENET_OTA_SELF_TEST_TIMEOUT_MS)) == 0)
    {
        ESP_LOGE(TAG, "新固件自检超时，开始回滚");
        esp_ota_mark_app_invalid_rollback_and_reboot();
        esp_restart();
    }

    s_rollback_handle = NULL;
    vTaskDelete(NULL);
}

/* 标记当前固件有效并取消回滚 */
static esp_err_t confirm_running_app(void)
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t state;
    esp_err_t result;

    if (running == NULL)
    {
        return ESP_FAIL;
    }

    result = esp_ota_get_state_partition(running, &state);
    if (result != ESP_OK || state != ESP_OTA_IMG_PENDING_VERIFY)
    {
        return ESP_OK;
    }

    result = esp_ota_mark_app_valid_cancel_rollback();
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "确认固件有效失败：%s", esp_err_to_name(result));
        return result;
    }

    if (s_rollback_handle != NULL)
    {
        xTaskNotifyGive(s_rollback_handle);
        s_rollback_handle = NULL;
    }
    ESP_LOGI(TAG, "当前固件确认有效，已取消回滚");
    return ESP_OK;
}

/* OTA 常驻工作线程：收到通知后完成一次检查或升级 */
static void ota_worker(void *argument)
{
    (void)argument;

    while (1)
    {
        const char *version;
        const esp_partition_t *partition;
        esp_err_t result = ESP_FAIL;
        static const char *types[] = {"2", "1"};

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* 等待启动阶段的天气等 HTTPS 请求释放 TLS 缓冲 */
        vTaskDelay(pdMS_TO_TICKS(3000));
        if (!datetime_is_time_synced())
        {
            datetime_wait_sntp_sync();
        }
        if (!datetime_is_time_synced())
        {
            ESP_LOGE(TAG, "系统时间未同步，无法安全校验证书");
            goto finish;
        }

        version = running_version();
        ESP_LOGI(TAG, "开始检查 OTA，当前版本=%s", version);

        for (int i = 0; i < 3; ++i)
        {
            result = upload_version();
            if (result == ESP_OK)
            {
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        if (result != ESP_OK)
        {
            ESP_LOGE(TAG, "版本上报失败，暂不确认新固件");
            goto finish;
        }

        result = confirm_running_app();
        if (result != ESP_OK)
        {
            goto finish;
        }

        result = ESP_ERR_NOT_FOUND;
        for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); ++i)
        {
            result = check_task(types[i], version);
            if (result != ESP_ERR_NOT_FOUND)
            {
                break;
            }
        }
        if (result == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGI(TAG, "没有待升级任务");
            goto finish;
        }
        if (result != ESP_OK)
        {
            goto finish;
        }
        if (strcmp(s_task.target, version) == 0)
        {
            ESP_LOGI(TAG, "目标版本与当前版本相同");
            goto finish;
        }

        partition = esp_ota_get_next_update_partition(NULL);
        if (partition == NULL)
        {
            ESP_LOGE(TAG, "没有可用的 OTA 分区");
            goto finish;
        }
        ESP_LOGI(TAG, "升级到 %s，写入分区 %s（0x%lx）", s_task.target,
                 partition->label, (unsigned long)partition->address);

        result = upload_status(s_task.id, 10);
        if (result != ESP_OK || download_image(s_task.id) != ESP_OK)
        {
            goto finish;
        }
        if (upload_status(s_task.id, 100) != ESP_OK)
        {
            ESP_LOGW(TAG, "升级完成，但最终进度上报失败");
        }

        ESP_LOGI(TAG, "升级完成，准备重启进入 %s", s_task.target);
        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_restart();

    finish:
        s_running = false;
    }
}

esp_err_t onenet_ota_init(void)
{
    const esp_partition_t *running;
    esp_ota_img_states_t state;
    esp_err_t result;

    if (s_initialized)
    {
        return ESP_OK;
    }

    if (s_worker_handle == NULL)
    {
        if (xTaskCreatePinnedToCoreWithCaps(
                ota_worker, "onenet_ota", OTA_WORKER_STACK_SIZE, NULL,
                OTA_WORKER_PRIORITY, &s_worker_handle, 1,
                MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT) != pdPASS)
        {
            ESP_LOGE(TAG, "创建 OTA 工作线程失败，内部内存=%u，最大块=%u",
                     (unsigned int)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                     (unsigned int)heap_caps_get_largest_free_block(
                         MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));
            return ESP_ERR_NO_MEM;
        }
    }

    s_initialized = true;
    running = esp_ota_get_running_partition();
    if (running == NULL)
    {
        return ESP_FAIL;
    }

    result = esp_ota_get_state_partition(running, &state);
    if (result != ESP_OK)
    {
        ESP_LOGI(TAG, "当前分区 %s 没有 OTA 状态", running->label);
        return ESP_OK;
    }
    if (state != ESP_OTA_IMG_PENDING_VERIFY)
    {
        ESP_LOGI(TAG, "当前分区 %s 已有效，state=%d", running->label, state);
        return ESP_OK;
    }

    ESP_LOGW(TAG, "新固件等待验证，超时时间=%d ms",
             ONENET_OTA_SELF_TEST_TIMEOUT_MS);
    if (xTaskCreatePinnedToCore(rollback_watchdog, "ota_rollback",
                                OTA_ROLLBACK_STACK_SIZE, NULL,
                                OTA_ROLLBACK_PRIORITY,
                                &s_rollback_handle, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "创建回滚看门狗失败");
        esp_ota_mark_app_invalid_rollback_and_reboot();
        esp_restart();
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

esp_err_t onenet_ota_start(void)
{
    if (s_running)
    {
        return ESP_OK;
    }
    if (!onenet_is_connected() || s_worker_handle == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    s_running = true;
    xTaskNotifyGive(s_worker_handle);
    return ESP_OK;
}
