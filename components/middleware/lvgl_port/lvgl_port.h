#ifndef __LVGL_PORT_H__
#define __LVGL_PORT_H__

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/* 初始化LVGL端口 */
esp_err_t lv_port_init(void);
/* 反初始化LVGL端口 */
void lv_port_deinit(void);
/* 初始化背光 */
void lv_port_backlight_init(void);
/* 设置背光亮度 */
void lv_port_set_backlight_brightness(int backlight_brightness);

#endif

