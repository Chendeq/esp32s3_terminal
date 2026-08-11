#ifndef __APP_UI_H__
#define __APP_UI_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "lvgl.h"
#include "gui_guider.h"

/* 绑定WiFi状态事件处理函数 */
void bind_wifi_status_event_handler(lv_ui *ui);

/* 更新home界面时间显示 */
void update_home_time_display(void);

/* 更新天气界面显示 */
void update_weather_display(void);

/* 更新日历界面显示 */
void update_calendar_from_time(void);

/* 更新串口接收显示 */
void update_serial_display(const char *data, size_t len);

void ai_ui_set_answer(const char *text);
void ai_ui_append_answer(const char *text);

/* 绑定音乐列表事件处理函数 */
void bind_music_event_handler(lv_ui *ui);

extern lv_ui guider_ui;

#endif
