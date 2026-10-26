#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl_port.h"
#include "app_task.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "onenet_ota.h"

static const char *TAG = "main";

void app_main(void)
{
    // 初始化NVS
    esp_err_t ret = ESP_OK;
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ret = onenet_ota_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to init OneNET OTA rollback protection: %s",
                 esp_err_to_name(ret));
    }

    ret = lv_port_init();
    if(ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to Init lvgl_port");
        return;
    }

    lvgl_port_lock(0);
    setup_ui(&guider_ui);
    lvgl_port_unlock();
    app_task_init();
    
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

