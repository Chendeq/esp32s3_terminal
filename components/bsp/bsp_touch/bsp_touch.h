#ifndef __BSP_TOUCH_H__
#define __BSP_TOUCH_H__

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/* 触摸硬件配置 */
#define BSP_TOUCH_I2C_SDA           GPIO_NUM_3
#define BSP_TOUCH_I2C_SCL           GPIO_NUM_18
#define BSP_TOUCH_I2C_PORT          I2C_NUM_0
#define BSP_TOUCH_I2C_CLOCK_HZ      (100 * 1000)

#define BSP_TOUCH_X_MAX             BSP_LCD_V_RES
#define BSP_TOUCH_Y_MAX             BSP_LCD_H_RES

/* 初始化触摸屏 */
esp_err_t bsp_touch_init(void);
/* 反初始化触摸屏 */
void bsp_touch_deinit(void);
/* 获取触摸屏句柄 */
void *bsp_touch_get_handle(void);


#endif

