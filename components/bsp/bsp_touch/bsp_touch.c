#include "bsp_touch.h"
#include "bsp_lcd.h"
#include "driver/i2c_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_ft5x06.h"
#include "esp_log.h"

static const char *TAG = "BSP_TOUCH";

static i2c_master_bus_handle_t s_i2c_bus_handle = NULL;
static esp_lcd_panel_io_handle_t s_touch_io_handle = NULL;
static esp_lcd_touch_handle_t s_touch_handle = NULL;

/* ================= 内部函数 ================= */

/**
 * @brief 初始化触摸屏I2C总线
 * 
 * @return esp_err_t 初始化结果
 */
static esp_err_t _touch_i2c_init(void)
{
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = BSP_TOUCH_I2C_PORT,
        .sda_io_num = BSP_TOUCH_I2C_SDA,
        .scl_io_num = BSP_TOUCH_I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags = {
            .enable_internal_pullup = false,
            .allow_pd = false,
        }
    };

    return i2c_new_master_bus(&i2c_bus_config, &s_i2c_bus_handle);
}

/**
 * @brief 初始化触摸屏驱动
 * 
 * @return esp_err_t 初始化结果
 */
static esp_err_t _touch_driver_init(void)
{
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_FT5x06_CONFIG();
    io_config.scl_speed_hz = BSP_TOUCH_I2C_CLOCK_HZ;

    esp_err_t ret = esp_lcd_new_panel_io_i2c_v2(s_i2c_bus_handle, &io_config, &s_touch_io_handle);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to create touch IO: %s", esp_err_to_name(ret));
        return ret;
    }

    esp_lcd_touch_config_t touch_config = {
        .x_max = BSP_TOUCH_X_MAX,
        .y_max = BSP_TOUCH_Y_MAX,
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = GPIO_NUM_NC,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 1,
            .mirror_x = 0,
            .mirror_y = 1,
        },
        .process_coordinates = NULL,
        .interrupt_callback = NULL,
    };

    return esp_lcd_touch_new_i2c_ft5x06(s_touch_io_handle, &touch_config, &s_touch_handle);
}

/* ==================== 外部接口函数 ==================== */

/**
 * @brief 初始化触摸屏
 * 
 * @return esp_err_t 初始化结果
 */
esp_err_t bsp_touch_init(void)
{
    ESP_LOGI(TAG, "Initialize FT6336U touch controller");

    esp_err_t ret = _touch_i2c_init();
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to initialize I2C bus");
        return ret;
    }

    ret = _touch_driver_init();
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to create FT5x06 touch driver");
        esp_lcd_panel_io_del(s_touch_io_handle);
        return ret;
    }

    ESP_LOGI(TAG, "Touch initialized successfully");
    return ESP_OK;
}

/**
 * @brief 反初始化触摸屏
 * 
 */
void bsp_touch_deinit(void)
{
    if (s_touch_handle) 
    {
        esp_lcd_touch_del(s_touch_handle);
        s_touch_handle = NULL;
    }
    if (s_touch_io_handle) 
    {
        esp_lcd_panel_io_del(s_touch_io_handle);
        s_touch_io_handle = NULL;
    }
    if (s_i2c_bus_handle) 
    {
        i2c_del_master_bus(s_i2c_bus_handle);
        s_i2c_bus_handle = NULL;
    }
}

/**
 * @brief 获取触摸屏句柄
 * 
 * @return void* 触摸屏句柄
 */
void *bsp_touch_get_handle(void)
{
    return s_touch_handle;
}
