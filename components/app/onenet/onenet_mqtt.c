#include "onenet_mqtt.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "onenet_ota.h"
#include "onenet_token.h"

#define TAG "onenet_mqtt"

#define TOPIC_MAX_SIZE 192
#define MQTT_QOS 1
#define MQTT_BUFFER_SIZE 2048

#define TOPIC_PROPERTY_POST_REPLY "/thing/property/post/reply"
#define TOPIC_PROPERTY_SET "/thing/property/set"
#define TOPIC_OTA_INFORM "/ota/inform"

/*
 * OneNET MQTT TLS 使用自签名服务器证书，CN 为 "OneNET MQTTS"。
 *
 * 获取方式：
 * openssl s_client -connect mqtts.heclouds.com:8883 \
 *     -servername mqtts.heclouds.com -showcerts
 *
 * 该证书有效期至 2049-05-21。OneNET 更换 MQTT 证书时需要同步更新。
 */
static const char ONENET_MQTT_CERT_PEM[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDOzCCAiOgAwIBAgIJAPCCNfxANtVEMA0GCSqGSIb3DQEBCwUAMDQxCzAJBgNV\n"
    "BAYTAkNOMQ4wDAYDVQQKDAVDTUlPVDEVMBMGA1UEAwwMT25lTkVUIE1RVFRTMB4X\n"
    "DTE5MDUyOTAxMDkyOFoXDTQ5MDUyMTAxMDkyOFowNDELMAkGA1UEBhMCQ04xDjAM\n"
    "BgNVBAoMBUNNSU9UMRUwEwYDVQQDDAxPbmVORVQgTVFUVFMwggEiMA0GCSqGSIb3\n"
    "DQEBAQUAA4IBDwAwggEKAoIBAQC/VvJ6lGWfy9PKdXKBdzY83OERB35AJhu+9jkx\n"
    "5d4SOtZScTe93Xw9TSVRKrFwu5muGgPusyAlbQnFlZoTJBZY/745MG6aeli6plpR\n"
    "r93G6qVN5VLoXAkvqKslLZlj6wXy70/e0GC0oMFzqSP0AY74icANk8dUFB2Q8usS\n"
    "UseRafNBcYfqACzF/Wa+Fu/upBGwtl7wDLYZdCm3KNjZZZstvVB5DWGnqNX9HkTl\n"
    "U9NBMS/7yph3XYU3mJqUZxryb8pHLVHazarNRppx1aoNroi+5/t3Fx/gEa6a5PoP\n"
    "ouH35DbykmzvVE67GUGpAfZZtEFE1e0E/6IB84PE00llvy3pAgMBAAGjUDBOMB0G\n"
    "A1UdDgQWBBTTi/q1F2iabqlS7yEoX1rbOsz5GDAfBgNVHSMEGDAWgBTTi/q1F2ia\n"
    "bqlS7yEoX1rbOsz5GDAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQAL\n"
    "aqJ2FgcKLBBHJ8VeNSuGV2cxVYH1JIaHnzL6SlE5q7MYVg+Ofbs2PRlTiWGMazC7\n"
    "q5RKVj9zj0z/8i3ScWrWXFmyp85ZHfuo/DeK6HcbEXJEOfPDvyMPuhVBTzuBIRJb\n"
    "41M27NdIVCdxP6562n6Vp0gbE8kN10q+ksw8YBoLFP0D1da7D5WnSV+nwEIP+F4a\n"
    "3ZX80bNt6tRj9XY0gM68mI60WXrF/qYL+NUz+D3Lw9bgDSXxpSN8JGYBR85BxBvR\n"
    "NNAhsJJ3yoAvbPUQ4m8J/CoVKKgcWymS1pvEHmF47pgzbbjm5bdthlIx+swdiGFa\n"
    "WzdhzTYwVkxBaU+xf/2w\n"
    "-----END CERTIFICATE-----\n";

static esp_mqtt_client_handle_t s_client;
static volatile bool s_connected;
static char s_token[ONENET_TOKEN_MAX_SIZE];
static onenet_property_set_handler_t s_property_handler;

/* 生成 OneNET 系统主题 */
static esp_err_t build_topic(char *topic, size_t size, const char *suffix)
{
    int length = snprintf(topic, size, "$sys/%s/%s%s", ONENET_PRODUCT_ID,
                          ONENET_DEVICE_NAME, suffix);
    return length >= 0 && (size_t)length < size ? ESP_OK
                                                : ESP_ERR_INVALID_SIZE;
}

/* 判断 MQTT 事件主题是否以指定后缀结尾 */
static bool topic_has_suffix(const esp_mqtt_event_handle_t event,
                             const char *suffix)
{
    size_t length = strlen(suffix);

    return event->topic != NULL && event->topic_len > 0 &&
           (size_t)event->topic_len >= length &&
           memcmp(event->topic + event->topic_len - length, suffix, length) == 0;
}

/* 发送属性设置或 OTA 通知回复 */
static esp_err_t send_reply(const char *suffix, const char *id, int code,
                            const char *message)
{
    char topic[TOPIC_MAX_SIZE];
    cJSON *reply = cJSON_CreateObject();
    char *payload;
    int message_id;
    esp_err_t result;

    if (reply == NULL)
    {
        return ESP_ERR_NO_MEM;
    }
    if (cJSON_AddStringToObject(reply, "id", id) == NULL ||
        cJSON_AddNumberToObject(reply, "code", code) == NULL ||
        cJSON_AddStringToObject(reply, "message", message) == NULL)
    {
        cJSON_Delete(reply);
        return ESP_ERR_NO_MEM;
    }

    payload = cJSON_PrintUnformatted(reply);
    cJSON_Delete(reply);
    if (payload == NULL)
    {
        return ESP_ERR_NO_MEM;
    }

    result = build_topic(topic, sizeof(topic), suffix);
    if (result == ESP_OK)
    {
        message_id = esp_mqtt_client_publish(s_client, topic, payload,
                                             (int)strlen(payload), MQTT_QOS, 0);
        result = message_id >= 0 ? ESP_OK : ESP_FAIL;
    }

    cJSON_free(payload);
    return result;
}

/* 复制 MQTT 事件数据，确保可作为字符串解析 */
static char *copy_payload(const esp_mqtt_event_handle_t event,
                          const char *name)
{
    char *payload;

    if (event->data == NULL || event->data_len <= 0)
    {
        return NULL;
    }
    if (event->total_data_len > event->data_len)
    {
        ESP_LOGW(TAG, "%s 消息被分片，当前实现忽略", name);
        return NULL;
    }

    payload = malloc((size_t)event->data_len + 1);
    if (payload == NULL)
    {
        return NULL;
    }
    memcpy(payload, event->data, (size_t)event->data_len);
    payload[event->data_len] = '\0';
    return payload;
}

/* 处理属性设置消息 */
static void handle_property_set(const esp_mqtt_event_handle_t event)
{
    char *payload = copy_payload(event, "属性设置");
    cJSON *root;
    cJSON *id;
    esp_err_t result = ESP_OK;
    int code = 200;
    const char *message = "success";

    if (payload == NULL)
    {
        return;
    }

    root = cJSON_Parse(payload);
    if (root == NULL)
    {
        ESP_LOGE(TAG, "属性设置 JSON 无效");
        free(payload);
        return;
    }

    id = cJSON_GetObjectItemCaseSensitive(root, "id");
    if (!cJSON_IsString(id))
    {
        ESP_LOGE(TAG, "属性设置消息缺少 id");
        goto cleanup;
    }

    if (s_property_handler != NULL)
    {
        result = s_property_handler(payload);
        if (result != ESP_OK)
        {
            code = 500;
            message = "failed";
            ESP_LOGE(TAG, "属性设置处理失败：%s", esp_err_to_name(result));
        }
    }

    if (send_reply("/thing/property/set_reply", id->valuestring, code,
                   message) != ESP_OK)
    {
        ESP_LOGE(TAG, "属性设置回复发送失败");
    }

cleanup:
    cJSON_Delete(root);
    free(payload);
}

/* 处理 OTA 通知，回复后唤醒 OTA 工作线程 */
static void handle_ota_inform(const esp_mqtt_event_handle_t event)
{
    char *payload = copy_payload(event, "OTA 通知");
    cJSON *root;
    cJSON *id;
    esp_err_t result;

    if (payload == NULL)
    {
        return;
    }

    root = cJSON_Parse(payload);
    if (root == NULL)
    {
        ESP_LOGE(TAG, "OTA 通知 JSON 无效");
        free(payload);
        return;
    }

    id = cJSON_GetObjectItemCaseSensitive(root, "id");
    if (!cJSON_IsString(id))
    {
        ESP_LOGE(TAG, "OTA 通知缺少 id");
        cJSON_Delete(root);
        free(payload);
        return;
    }

    result = send_reply("/ota/inform_reply", id->valuestring, 200, "success");
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "OTA 通知回复发送失败");
    }
    cJSON_Delete(root);
    free(payload);

    result = onenet_ota_start();
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "启动 OTA 检查失败：%s", esp_err_to_name(result));
    }
}

/* 订阅属性回复、属性设置和 OTA 通知主题 */
static esp_err_t subscribe_topics(void)
{
    static const char *suffixes[] = {
        TOPIC_PROPERTY_POST_REPLY,
        TOPIC_PROPERTY_SET,
        TOPIC_OTA_INFORM,
    };
    char topic[TOPIC_MAX_SIZE];
    esp_err_t result = ESP_OK;

    for (size_t i = 0; i < sizeof(suffixes) / sizeof(suffixes[0]); ++i)
    {
        if (build_topic(topic, sizeof(topic), suffixes[i]) != ESP_OK ||
            esp_mqtt_client_subscribe_single(s_client, topic, MQTT_QOS) < 0)
        {
            ESP_LOGE(TAG, "订阅主题失败：%s", topic);
            result = ESP_FAIL;
        }
        else
        {
            ESP_LOGI(TAG, "已订阅：%s", topic);
        }
    }
    return result;
}

/* MQTT 事件处理 */
static void mqtt_event_handler(void *arg, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    (void)arg;
    (void)base;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_BEFORE_CONNECT:
        ESP_LOGI(TAG, "正在连接 OneNET MQTT");
        break;

    case MQTT_EVENT_CONNECTED:
        s_connected = true;
        ESP_LOGI(TAG, "OneNET MQTT 已连接，session_present=%d",
                 event->session_present);
        if (subscribe_topics() != ESP_OK)
        {
            ESP_LOGW(TAG, "部分主题订阅失败");
        }
        if (onenet_ota_start() != ESP_OK)
        {
            ESP_LOGW(TAG, "启动 OTA 检查失败");
        }
        break;

    case MQTT_EVENT_DISCONNECTED:
        s_connected = false;
        ESP_LOGW(TAG, "OneNET MQTT 已断开");
        break;

    case MQTT_EVENT_DATA:
        if (topic_has_suffix(event, TOPIC_PROPERTY_SET))
        {
            handle_property_set(event);
        }
        else if (topic_has_suffix(event, TOPIC_OTA_INFORM))
        {
            handle_ota_inform(event);
        }
        else if (topic_has_suffix(event, TOPIC_PROPERTY_POST_REPLY) &&
                 event->data != NULL && event->data_len > 0)
        {
            ESP_LOGI(TAG, "属性上报回复：%.*s", event->data_len, event->data);
        }
        break;

    case MQTT_EVENT_ERROR:
        if (event->error_handle != NULL)
        {
            ESP_LOGE(TAG, "MQTT 错误：type=%d，tls=0x%x，stack=%d，socket=%d",
                     event->error_handle->error_type,
                     (unsigned int)event->error_handle->esp_tls_last_esp_err,
                     event->error_handle->esp_tls_stack_err,
                     event->error_handle->esp_transport_sock_errno);
        }
        else
        {
            ESP_LOGE(TAG, "MQTT 未知错误");
        }
        break;

    default:
        break;
    }
}

esp_err_t onenet_start(void)
{
    esp_mqtt_client_config_t config = {0};
    esp_err_t result;

    if (s_client != NULL)
    {
        return ESP_OK;
    }

    if (dev_token_generate_ex(s_token, sizeof(s_token), SIG_METHOD_SHA256,
                              ONENET_TOKEN_EXPIRE_TIME, ONENET_PRODUCT_ID,
                              ONENET_DEVICE_NAME, ONENET_ACCESS_KEY) != 0)
    {
        ESP_LOGE(TAG, "生成 OneNET Token 失败");
        return ESP_FAIL;
    }

    config.broker.address.uri = ONENET_MQTT_BROKER_URI;
    config.broker.verification.certificate = ONENET_MQTT_CERT_PEM;
    config.broker.verification.common_name = "OneNET MQTTS";
    config.credentials.client_id = ONENET_DEVICE_NAME;
    config.credentials.username = ONENET_PRODUCT_ID;
    config.credentials.authentication.password = s_token;
    config.session.protocol_ver = MQTT_PROTOCOL_V_3_1_1;
    config.session.keepalive = 60;
    config.network.reconnect_timeout_ms = 5000;
    config.network.timeout_ms = 10000;
    config.buffer.size = MQTT_BUFFER_SIZE;
    config.buffer.out_size = MQTT_BUFFER_SIZE;

    s_client = esp_mqtt_client_init(&config);
    if (s_client == NULL)
    {
        return ESP_ERR_NO_MEM;
    }

    result = esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID,
                                            mqtt_event_handler, NULL);
    if (result == ESP_OK)
    {
        result = esp_mqtt_client_start(s_client);
    }
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "启动 OneNET MQTT 失败：%s", esp_err_to_name(result));
        esp_mqtt_client_destroy(s_client);
        s_client = NULL;
        return result;
    }

    ESP_LOGI(TAG, "OneNET MQTT 客户端已启动");
    return ESP_OK;
}

esp_err_t onenet_stop(void)
{
    esp_err_t result;

    if (s_client == NULL)
    {
        return ESP_OK;
    }

    s_connected = false;
    result = esp_mqtt_client_stop(s_client);
    if (result == ESP_OK)
    {
        result = esp_mqtt_client_destroy(s_client);
        s_client = NULL;
    }
    return result;
}

bool onenet_is_connected(void)
{
    return s_connected;
}

void onenet_register_property_set_handler(onenet_property_set_handler_t handler)
{
    s_property_handler = handler;
}

esp_err_t onenet_post_property_data(const char *data)
{
    char topic[TOPIC_MAX_SIZE];
    size_t length;
    int message_id;
    esp_err_t result;

    if (data == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_client == NULL || !s_connected)
    {
        return ESP_ERR_INVALID_STATE;
    }

    length = strlen(data);
    if (length > INT_MAX)
    {
        return ESP_ERR_INVALID_SIZE;
    }

    result = build_topic(topic, sizeof(topic), "/thing/property/post");
    if (result != ESP_OK)
    {
        return result;
    }

    message_id = esp_mqtt_client_publish(s_client, topic, data, (int)length,
                                         MQTT_QOS, 0);
    if (message_id < 0)
    {
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "属性数据已发送，msg_id=%d", message_id);
    return ESP_OK;
}
