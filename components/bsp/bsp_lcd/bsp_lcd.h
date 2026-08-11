#ifndef __BSP_LCD_H_
#define __BSP_LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/* LCD硬件配置 */
#define BSP_LCD_H_RES               320
#define BSP_LCD_V_RES               240
#define BSP_LCD_SPI_HOST            SPI2_HOST
#define BSP_LCD_SPI_MOSI            GPIO_NUM_7
#define BSP_LCD_SPI_CLK             GPIO_NUM_15
#define BSP_LCD_SPI_CS              GPIO_NUM_4
#define BSP_LCD_DC_PIN              GPIO_NUM_6
#define BSP_LCD_RST_PIN             GPIO_NUM_5
#define BSP_LCD_BL_PIN              GPIO_NUM_16
#define BSP_LCD_TRANSFER_SIZE       (BSP_LCD_H_RES * 40)

/* LCD初始化 */
esp_err_t bsp_lcd_init(void);
/* LCD反初始化 */
void bsp_lcd_deinit(void);
/* LCD背光初始化 */
void bsp_lcd_backlight_init(void);
/* 设置背光亮度 */
void bsp_lcd_set_backlight_brightness(int backlight_brightness);
/* 获取LCD面板句柄 */
void* bsp_lcd_get_panel_handle(void);
/* 获取LCD IO句柄 */
void* bsp_lcd_get_io_handle(void);

#endif

