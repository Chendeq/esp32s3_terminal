#include "bsp_lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_st7789.h"

static const char *TAG = "BSP_LCD";

static bool s_is_backlight_initialized = false;              // 背光是否已初始化
static esp_lcd_panel_io_handle_t s_lcd_io_handle = NULL;     // LCD IO句柄
static esp_lcd_panel_handle_t s_lcd_panel_handle = NULL;     // LCD面板句柄

/* ===================== 内部函数 ===================== */

/**
 * @brief 初始化SPI总线
 * 
 * @return esp_err_t 初始化结果
 */
static esp_err_t _lcd_spi_bus_init(void)
{
    spi_bus_config_t bus_cfg = ST7789_PANEL_BUS_SPI_CONFIG(
        BSP_LCD_SPI_CLK,
        BSP_LCD_SPI_MOSI,
        BSP_LCD_TRANSFER_SIZE * sizeof(uint16_t)
    );

    esp_err_t ret = spi_bus_initialize(BSP_LCD_SPI_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "SPI bus init failed: %s", esp_err_to_name(ret));
    }
    return ret;
}

/**
 * @brief 创建LCD面板
 * 
 * @return esp_err_t 创建结果
 */
static esp_err_t _lcd_panel_create(void)
{
    esp_lcd_panel_io_spi_config_t io_config = ST7789_PANEL_IO_SPI_CONFIG(
        BSP_LCD_SPI_CS,
        BSP_LCD_DC_PIN,
        NULL,
        NULL
    );

    esp_err_t ret = esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)BSP_LCD_SPI_HOST, &io_config, &s_lcd_io_handle);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Panel IO create failed: %s", esp_err_to_name(ret));
        return ret;
    }

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BSP_LCD_RST_PIN,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };

    ret = esp_lcd_new_panel_st7789(s_lcd_io_handle, &panel_config, &s_lcd_panel_handle);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Panel create failed: %s", esp_err_to_name(ret));
        esp_lcd_panel_io_del(s_lcd_io_handle);
        s_lcd_io_handle = NULL;
    }
    return ret;
}

/**
 * @brief 配置LCD面板
 * 
 * @return esp_err_t 配置结果
 */
static esp_err_t _lcd_panel_config(void)
{
    esp_err_t ret = esp_lcd_panel_reset(s_lcd_panel_handle);
    if (ret != ESP_OK) 
        return ret;

    ret = esp_lcd_panel_init(s_lcd_panel_handle);
    if (ret != ESP_OK) 
        return ret;

    ret = esp_lcd_panel_swap_xy(s_lcd_panel_handle, true);
    if (ret != ESP_OK) 
        return ret;

    ret = esp_lcd_panel_mirror(s_lcd_panel_handle, false, true);
    if (ret != ESP_OK) 
        return ret;

    /* 初始化背光 */
    gpio_set_direction(BSP_LCD_BL_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BSP_LCD_BL_PIN, 1);

    ret = esp_lcd_panel_disp_on_off(s_lcd_panel_handle, true);
    return ret;
}

/* ===================== 外部接口函数 ===================== */

/**
 * @brief 初始化LCD面板
 * 
 * @return esp_err_t 初始化结果
 */
esp_err_t bsp_lcd_init(void)
{
    ESP_LOGI(TAG, "Initialize ST7789 LCD");

    /* 初始化SPI总线 */
    esp_err_t ret = _lcd_spi_bus_init();
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "SPI bus init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* 创建面板 */
    ret = _lcd_panel_create();
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Panel create failed: %s", esp_err_to_name(ret));
        spi_bus_free(BSP_LCD_SPI_HOST);
        return ret;
    }

    /* 配置面板 */
    ret = _lcd_panel_config();
    if (ret != ESP_OK) 
    {
        bsp_lcd_deinit();
        ESP_LOGE(TAG, "Panel config failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "LCD initialized successfully");
    return ESP_OK;
}

/**
 * @brief 反初始化LCD面板
 * 
 * @return esp_err_t 反初始化结果
 */
void bsp_lcd_deinit(void)
{
    if (s_lcd_panel_handle) 
    {
        esp_lcd_panel_del(s_lcd_panel_handle);
        s_lcd_panel_handle = NULL;
    }
    if (s_lcd_io_handle) 
    {
        esp_lcd_panel_io_del(s_lcd_io_handle);
        s_lcd_io_handle = NULL;
    }
    spi_bus_free(BSP_LCD_SPI_HOST);
    gpio_set_level(BSP_LCD_BL_PIN, 0);
}

/**
 * @brief 初始化屏幕背光所在引脚的PWM调光
 * 
 */
void bsp_lcd_backlight_init(void)
{
    if(s_is_backlight_initialized)
        return;

    ESP_LOGI(TAG, "Init Backlight");

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .gpio_num = BSP_LCD_BL_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    s_is_backlight_initialized = true;
    ESP_LOGI(TAG, "Backlight initialized");
}

/**
 * @brief 设置屏幕亮度
 * 
 * @param backlight_brightness 亮度，范围 0-100，0 表示完全关闭，100 表示完全开启
 */
void bsp_lcd_set_backlight_brightness(int backlight_brightness)
{
    if(!s_is_backlight_initialized)
    {
        return;
    }
    
    if (backlight_brightness < 0) 
        backlight_brightness = 0;
    if (backlight_brightness > 100) 
        backlight_brightness = 100;
    
    uint32_t duty = (backlight_brightness * 255) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

/**
 * @brief 获取LCD面板句柄
 * 
 * @return void* LCD面板句柄
 */
void* bsp_lcd_get_panel_handle(void)
{
    return s_lcd_panel_handle;
}

/**
 * @brief 获取LCD IO句柄
 * 
 * @return void* LCD IO句柄
 */
void* bsp_lcd_get_io_handle(void)
{
    return s_lcd_io_handle;
}
