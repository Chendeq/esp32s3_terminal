#include "lvgl_port.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "LVGL_PORT";

static lv_disp_t *s_lvgl_disp = NULL;
static lv_indev_t *s_lvgl_touch_indev = NULL;

/* ==================== 内部函数 ==================== */

static esp_err_t _lv_port_add_display(void)
{
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = bsp_lcd_get_io_handle(),
        .panel_handle = bsp_lcd_get_panel_handle(),
        .buffer_size = BSP_LCD_H_RES * 20,
        .double_buffer = true,
        .hres = BSP_LCD_H_RES,
        .vres = BSP_LCD_V_RES,
        .monochrome = false,
        .rotation = {
            .swap_xy = true,
            .mirror_x = false,
            .mirror_y = true,
        },
        .flags = {
            .buff_dma = true,          //DMA和SPIRAM不能同时启用，这里选择使用内部双缓冲+DMA
            .buff_spiram = false,
        }
    };

    s_lvgl_disp = lvgl_port_add_disp(&disp_cfg);
    if (s_lvgl_disp == NULL) 
    {
        ESP_LOGE(TAG, "Failed to add display");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Display added to LVGL successfully");
    return ESP_OK;
}

static esp_err_t _lv_port_add_touch(void)
{
    if (bsp_touch_get_handle() == NULL) 
    {
        ESP_LOGW(TAG, "Touch handle is NULL, skipping touch device addition");
        return ESP_OK;
    }

    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = s_lvgl_disp,
        .handle = bsp_touch_get_handle(),
    };

    s_lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);
    if (s_lvgl_touch_indev == NULL) 
    {
        ESP_LOGE(TAG, "Failed to add touch device to LVGL");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Touch device added to LVGL successfully");
    return ESP_OK;
}

/* ==================== 外部接口函数 ==================== */

/**
 * @brief 初始化LVGL端口
 * 
 * @return esp_err_t ESP_OK 成成功，其他错误码失败
 */
esp_err_t lv_port_init(void)
{
    ESP_LOGI(TAG, "Initialize LVGL port");

    /* 初始化BSP层硬件 */
    esp_err_t ret = bsp_lcd_init();
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to initialize LCD");
        return ret;
    }

    ret = bsp_touch_init();
    if (ret != ESP_OK) 
    {
        ESP_LOGW(TAG, "Failed to initialize touch, continuing without touch");
        return ret;
    }

    /* 配置LVGL任务 */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 5,
        .task_stack = 1024 * 15,
        .task_affinity = 1,
        .task_max_sleep_ms = 33,
        .timer_period_ms = 5,
    };

    ret = lvgl_port_init(&lvgl_cfg);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to initialize LVGL port");
        lv_port_deinit();
        return ret;
    }

    /* 添加显示和触摸 */
    ret = _lv_port_add_display();
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to add display to LVGL");
        return ret;
    }

    if (bsp_touch_get_handle() != NULL) 
    {
        ret = _lv_port_add_touch();
        if (ret != ESP_OK) 
        {
            ESP_LOGW(TAG, "Failed to add touch to LVGL");
            return ret;
        }
    }

    ESP_LOGI(TAG, "LVGL port initialized successfully");
    return ESP_OK;
}

/**
 * @brief 反初始化LVGL端口
 * 
 */
void lv_port_deinit(void)
{
    if (s_lvgl_touch_indev) 
    {
        lvgl_port_remove_touch(s_lvgl_touch_indev);
        s_lvgl_touch_indev = NULL;
    }

    if (s_lvgl_disp) 
    {
        lvgl_port_remove_disp(s_lvgl_disp);
        s_lvgl_disp = NULL;
    }

    lvgl_port_deinit();

    bsp_touch_deinit();
    bsp_lcd_deinit();

}

/**
 * @brief 初始化背光
 * 
 */
void lv_port_backlight_init(void)
{
    bsp_lcd_backlight_init();
}

/**
 * @brief 设置背光亮度
 * 
 * @param backlight_brightness 背光亮度值，范围0-100
 */
void lv_port_set_backlight_brightness(int backlight_brightness)
{
    bsp_lcd_set_backlight_brightness(backlight_brightness);
}

