#include "app_ui.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl/lvgl.h"
#include "lvgl_port.h"
#include "wifi.h"
#include "datetime.h"
#include "weather.h"
#include "gui_guider.h"
#include "esp_lvgl_port.h"
#include "serial.h"
#include <stddef.h>
#include <stdio.h>
#include "music.h"
#include "xfyun.h"

static const char *TAG = "app_ui";

static char s_music_name_cache[MUSIC_MAX_FILES][MP3_NAME_MAX_LEN];
static uint16_t s_music_count = 0;
static lv_timer_t *s_music_ui_timer = NULL;
static uint32_t s_last_music_elapsed_ms = UINT32_MAX;
static uint32_t s_last_music_total_ms = UINT32_MAX;
static uint8_t s_last_music_progress = 0xFF;
static int16_t s_last_music_lyric_index = INT16_MIN;

lv_ui guider_ui;

/* ===================== 时间显示 UI 更新=================== */

/**
 * @brief 更新home界面时间显示
 */
void update_home_time_display(void)
{
    if (!lvgl_port_lock(0)) 
    {
        return;
    }

    // 检查 home 界面是否有效
    if (guider_ui.home && lv_obj_is_valid(guider_ui.home)) 
    {
        char time_buf[16] = {0};
        char date_buf[32] = {0};
        char week_buf[16] = {0};
        datetime_get_current_time_str(time_buf, sizeof(time_buf), date_buf, sizeof(date_buf), week_buf, sizeof(week_buf));

        if (guider_ui.home_time_label && lv_obj_is_valid(guider_ui.home_time_label)) 
        {
            lv_label_set_text(guider_ui.home_time_label, time_buf);
        }
        if (guider_ui.home_date_label && lv_obj_is_valid(guider_ui.home_date_label)) 
        {
            lv_label_set_text(guider_ui.home_date_label, date_buf);
        }
        if (guider_ui.home_week_label && lv_obj_is_valid(guider_ui.home_week_label)) 
        {
            lv_label_set_text(guider_ui.home_week_label, week_buf);
        }
    }

    lvgl_port_unlock();
}

/**
 * @brief 根据系统时间更新日历控件的显示和高亮日期
 */
void update_calendar_from_time(void)
{
    if (!guider_ui.calendar_calendar_1 || !lv_obj_is_valid(guider_ui.calendar_calendar_1)) {
        return;
    }
    
    if (!datetime_is_time_synced()) {
        ESP_LOGW(TAG, "Time not synced, skip calendar update");
        return;
    }
    
    int year, month, day;
    datetime_get_current_date(&year, &month, &day);
    
    if (!lvgl_port_lock(0)) 
        return;
    
    // 设置显示的年月（确保日历翻到当前月）
    lv_calendar_set_showed_date(guider_ui.calendar_calendar_1, year, month);
    // 设置今日日期
    lv_calendar_set_today_date(guider_ui.calendar_calendar_1, year, month, day);
    // 设置高亮日期（可设置一个数组，例如高亮明日）
    lv_calendar_date_t highlighted[1];
    highlighted[0].year = year;
    highlighted[0].month = month;
    highlighted[0].day = day + 1;
    lv_calendar_set_highlighted_dates(guider_ui.calendar_calendar_1, highlighted, 1);

    lvgl_port_unlock();
}

/* ===================== END 时间显示 UI 更新=================== */

/* ===================== 天气显示 UI 更新=================== */

void update_weather_display(void)
{
    weather_cache_t cache;
    weather_get_cached(&cache);
    if (!cache.has_data) 
        return;

    if (!guider_ui.home && !guider_ui.weather) {
        return;
    }

    if (!lvgl_port_lock(0)) 
        return;

    // 1. 更新 home 页天气（如果界面存在）
    if (guider_ui.home && lv_obj_is_valid(guider_ui.home)) 
    {
        if (guider_ui.home_weather_status_label && lv_obj_is_valid(guider_ui.home_weather_status_label))
            lv_label_set_text(guider_ui.home_weather_status_label, cache.weather);
        if (guider_ui.home_today_weather_label && lv_obj_is_valid(guider_ui.home_today_weather_label))
            lv_label_set_text_fmt(guider_ui.home_today_weather_label, "%s℃", cache.temp);
        if (guider_ui.home_home_address_label && lv_obj_is_valid(guider_ui.home_home_address_label))
            lv_label_set_text(guider_ui.home_home_address_label, cache.city);
    }

    // 2. 更新 weather 页（如果界面存在）
    if (guider_ui.weather && lv_obj_is_valid(guider_ui.weather)) 
    {
        if (guider_ui.weather_last_time_label && lv_obj_is_valid(guider_ui.weather_last_time_label))
            lv_label_set_text(guider_ui.weather_last_time_label, cache.last_update);
        if (guider_ui.weather_address_label && lv_obj_is_valid(guider_ui.weather_address_label))
            lv_label_set_text(guider_ui.weather_address_label, cache.city);
        // 三天预报
        lv_obj_t *date_labels[] = {guider_ui.weather_date_label_1, guider_ui.weather_date_label_2, guider_ui.weather_date_label_3};
        lv_obj_t *status_labels[] = {guider_ui.weather_status_label_1, guider_ui.weather_status_label_2, guider_ui.weather_status_label_3};
        lv_obj_t *temp_labels[] = {guider_ui.weather_temp_label_1, guider_ui.weather_temp_label_2, guider_ui.weather_temp_label_3};
        for (int i = 0; i < 3; i++) 
        {
            if (date_labels[i] && lv_obj_is_valid(date_labels[i]))
                lv_label_set_text(date_labels[i], cache.forecast_date[i]);
            if (status_labels[i] && lv_obj_is_valid(status_labels[i]))
                lv_label_set_text(status_labels[i], cache.forecast_weather[i]);
            if (temp_labels[i] && lv_obj_is_valid(temp_labels[i]))
                lv_label_set_text(temp_labels[i], cache.forecast_temp[i]);
        }
    }

    lvgl_port_unlock();
}

/* ===================== END 天气显示 UI 更新=================== */

/**
 * @brief 设置WiFi、音乐列表按钮样式
 * @param btn LVGL按钮对象柄
 */
static void style_list_btn(lv_obj_t *btn)
{
    lv_obj_set_style_text_font(btn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x2fdaff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(btn, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
}

/* ===================== WIFI UI更新=================== */

/**
 * @brief 列表按钮删除事件回调，释放保存的 SSID 字符串
 * @param e LVGL 事件
 */
static void wifi_list_btn_delete_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    void *ssid_ptr = lv_obj_get_user_data(btn);
    if (ssid_ptr) {
        lv_mem_free(ssid_ptr);
        lv_obj_set_user_data(btn, NULL);
    }
}

/**
 * @brief WiFi列表项点击事件处理函数
 * @param e LVGL事件句柄
 */
static void wifi_list_item_click_event_handler(lv_event_t *e)
{
    lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (!ui || !btn)
        return;

    const char *ssid = (const char *)lv_obj_get_user_data(btn);
    if (!ssid || ssid[0] == '\0')
        return;

    // 保存选中的SSID
    strncpy(wifi_req.ssid, ssid, sizeof(wifi_req.ssid) - 1);
    wifi_req.ssid[sizeof(wifi_req.ssid) - 1] = '\0';

    memset(wifi_req.password, 0, sizeof(wifi_req.password));

    lv_obj_move_foreground(guider_ui.setting_pwd_cont);
    lv_obj_clear_flag(guider_ui.setting_pwd_cont, LV_OBJ_FLAG_HIDDEN);
    lv_textarea_set_text(guider_ui.setting_pwd_ta, "");
    lv_label_set_text(guider_ui.setting_wifi_id_label, wifi_req.ssid);
}

/**
 * @brief WiFi列表项刷新函数
 */
static void wifi_list_item_refresh(void)
{
    ESP_LOGI(TAG, "Refresh list, scan_result=[\n%s]", wifi_req.scan_result);
    if(!guider_ui.setting_wifi_list || !lv_obj_is_valid(guider_ui.setting_wifi_list))
        return;

    // 清空旧列表
    lv_obj_clean(guider_ui.setting_wifi_list);

    // 解析 scan_result，获取有效SSID行数
    const char *ptr = wifi_req.scan_result;
    int line_count = 0;
    while (*ptr) {
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\r') 
            ptr++; // 跳过空白
        if (*ptr == '\0') 
            break;
        const char *start = ptr;
        while (*ptr && *ptr != '\n')  
            ptr++;
        if (ptr > start) 
            line_count++;
        if (*ptr == '\n') 
            ptr++;
    }

    // 若无可用AP，显示提示
    if (strcmp(wifi_req.scan_result, "附近无WiFi网络\n") == 0 || line_count == 0) 
    {
        lv_obj_t *btn = lv_list_add_btn(guider_ui.setting_wifi_list, LV_SYMBOL_WARNING, "附近无WiFi网络");
        if (btn) 
        {
            style_list_btn(btn);
            lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        }
        return;
    }

    // 重新解析并创建每个AP的按钮
    ptr = wifi_req.scan_result;
    int idx = 0;
    while (*ptr && idx < WIFI_SCAN_MAX_NUM) 
    {
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\r') 
            ptr++;
        if (*ptr == '\0') 
            break;
        const char *start = ptr;
        while (*ptr && *ptr != '\n') ptr++;
        size_t len = (size_t)(ptr - start);
        if (len > 0) 
        {
            char ssid[33] = {0};
            if (len >= sizeof(ssid)) 
                len = sizeof(ssid) - 1;
            memcpy(ssid, start, len);
            ssid[len] = '\0';

            lv_obj_t *btn = lv_list_add_btn(guider_ui.setting_wifi_list, LV_SYMBOL_WIFI, ssid);
            if (btn) 
            {
                char *copy = lv_mem_alloc(strlen(ssid) + 1);
                if (copy) 
                {
                    strcpy(copy, ssid);
                    lv_obj_set_user_data(btn, copy);
                    style_list_btn(btn);
                    lv_obj_add_event_cb(btn, wifi_list_item_click_event_handler, LV_EVENT_CLICKED, &guider_ui);
                    lv_obj_add_event_cb(btn, wifi_list_btn_delete_cb, LV_EVENT_DELETE, NULL);
                }
            }
            idx++;
        }
        if (*ptr == '\n') 
            ptr++;
    }
}

/**
 * @brief WiFi状态事件处理函数
 * @param event_id 事件类型
*/
static void wifi_status_event_handler(wifi_event_id_t event_id)
{
    if(lvgl_port_lock(0))
    {
        switch(event_id)
        {
            case WIFI_EVT_CONNECTED:
            {
                // 更新UI显示
                if (guider_ui.setting_wifi_ssid_label && lv_obj_is_valid(guider_ui.setting_wifi_ssid_label)) 
                {
                    lv_label_set_text_fmt(guider_ui.setting_wifi_ssid_label, "已连接 %s", wifi_req.ssid);
                }
                // 隐藏密码输入框
                if(guider_ui.setting_pwd_cont && lv_obj_is_valid(guider_ui.setting_pwd_cont))
                {
                    lv_obj_add_flag(guider_ui.setting_pwd_cont, LV_OBJ_FLAG_HIDDEN);
                }

                // 同步时间
                if(wifi_connect_state() == true)
                {
                    if(!datetime_is_time_synced())
                    {
                        datetime_wait_sntp_sync();
                    }
                }
                break;
            }
            case WIFI_EVT_DISCONNECTED:
            {
                if(guider_ui.setting_wifi_ssid_label && lv_obj_is_valid(guider_ui.setting_wifi_ssid_label))
                {
                    lv_label_set_text(guider_ui.setting_wifi_ssid_label, "未连接");
                }
                break;
            }
            case WIFI_EVT_SCAN_DONE:
            {
                // 刷新WiFi列表
                wifi_list_item_refresh();
                break;
            }
        }
    }
    lvgl_port_unlock();
}

/**
 * @brief 绑定WiFi状态事件处理函数
 * @param ui LVGL UI句柄
 */
void bind_wifi_status_event_handler(lv_ui *ui)
{
    (void)ui;
    wifi_register_handler(wifi_status_event_handler);
}

/* ===================== END WIFI UI更新=================== */

/* ===================== SERIAL UI更新 ==================== */

/**
 * @brief 将接收到的串口数据追加到 RX 文本框（线程安全）
 * @param data 数据指针（不要求 '\0' 结尾）
 * @param len  数据长度
 */
void update_serial_display(const char *data, size_t len)
{
    if (!guider_ui.serial_rx_ta || !lv_obj_is_valid(guider_ui.serial_rx_ta))
        return;

    if (!lvgl_port_lock(0))
        return;

    // 获取当前文本长度，防止超出最大限制（3000）
    const char *old_text = lv_textarea_get_text(guider_ui.serial_rx_ta);
    size_t old_len = strlen(old_text);
    const size_t MAX_LEN = 3000;  // 与 setup_scr_serial.c 中设置的最大长度一致
    if (old_len + len > MAX_LEN) {
        // 超出时丢弃最旧部分，保留后半段
        size_t keep = MAX_LEN - len;        // 计算需要保留的文本长度
        if (keep > 0 && keep < old_len) {   // 如果需要保留的文本长度大于0且小于当前文本长度
            const char *new_start = old_text + (old_len - keep);   // 计算新起始位置
            lv_textarea_set_text(guider_ui.serial_rx_ta, new_start);   // 设置新文本
        } else {
            lv_textarea_set_text(guider_ui.serial_rx_ta, "");
        }
    }

    // 追加新数据
    // 为保证安全，复制一份临时字符串
    char chunk[129];
    size_t chunk_len = len > 128 ? 128 : len;
    memcpy(chunk, data, chunk_len);
    chunk[chunk_len] = '\0';
    lv_textarea_add_text(guider_ui.serial_rx_ta, chunk);
    // 自动滚动到底部
    lv_textarea_set_cursor_pos(guider_ui.serial_rx_ta, LV_TEXTAREA_CURSOR_LAST);

    lvgl_port_unlock();
}

/* ===================== END SERIAL UI更新 ==================== */

/* ===================== AI UI更新 ==================== */

void ai_ui_set_answer(const char *text)
{
    if (!guider_ui.ai1_airx_ta || !lv_obj_is_valid(guider_ui.ai1_airx_ta))
        return;

    if (!lvgl_port_lock(0))
        return;

    lv_textarea_set_text(guider_ui.ai1_airx_ta, text ? text : "");  
    lv_textarea_set_cursor_pos(guider_ui.ai1_airx_ta, LV_TEXTAREA_CURSOR_LAST);   // 自动滚动到底部
    lvgl_port_unlock();
}

void ai_ui_append_answer(const char *text)
{
    if (!text || !guider_ui.ai1_airx_ta || !lv_obj_is_valid(guider_ui.ai1_airx_ta))
        return;

    if (!lvgl_port_lock(0))
        return;

    // 检查是否超出最大长度
    const size_t max_len = 3000;
    const char *old_text = lv_textarea_get_text(guider_ui.ai1_airx_ta);   // 获取当前文本内容
    size_t old_len = old_text ? strlen(old_text) : 0;   // 获取当前文本长度
    size_t add_len = strlen(text);   // 获取需要追加的文本长度    
    if (old_len + add_len > max_len) {   // 如果超出最大长度, 清空文本框
        lv_textarea_set_text(guider_ui.ai1_airx_ta, "");
    }
    lv_textarea_add_text(guider_ui.ai1_airx_ta, text);
    lv_textarea_set_cursor_pos(guider_ui.ai1_airx_ta, LV_TEXTAREA_CURSOR_LAST);   // 自动滚动到底部
    lvgl_port_unlock();
}

/* ===================== END AI UI更新 ==================== */

/* ======================== MUSIC UI更新 ======================= */

/**
 * @brief 音乐列表按钮点击事件处理函数
 * @param e LVGL事件对象
 */
static void music_list_item_click_event_handler(lv_event_t *e)
{
    lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (!ui || !btn) 
        return;

    uint32_t token = (uint32_t)(uintptr_t)lv_obj_get_user_data(btn);
    if (token == 0) 
        return;

    uint16_t index = (uint16_t)(token - 1);
    if (index >= music_get_count()) 
        return;

    // 发送播放指定索引的音乐命令
    music_cmd_msg_t msg = { .cmd = MUSIC_CMD_PLAY_INDEX, .data.index = index };
    music_send_cmd(&msg);
}

/**
 * @brief 刷新音乐列表
 */
static void music_format_time(uint32_t time_ms, char *buf, size_t buf_size)
{
    if (!buf || buf_size == 0) {
        return;
    }

    uint32_t total_seconds = time_ms / 1000U;
    uint32_t minutes = total_seconds / 60U;
    uint32_t seconds = total_seconds % 60U;
    snprintf(buf, buf_size, "%02lu:%02lu", (unsigned long)minutes, (unsigned long)seconds);
}

static void music_update_progress_ui(void)
{
    char elapsed_buf[12] = {0};
    char total_buf[12] = {0};

    music_format_time(music_get_elapsed_time_ms(), elapsed_buf, sizeof(elapsed_buf));   // 格式化已播放时间
    music_format_time(music_get_total_time_ms(), total_buf, sizeof(total_buf));         // 格式化总时间

    if (guider_ui.music_time_play_label && lv_obj_is_valid(guider_ui.music_time_play_label))
    {
        lv_label_set_text(guider_ui.music_time_play_label, elapsed_buf);
    }

    if (guider_ui.music_totaltime_label && lv_obj_is_valid(guider_ui.music_totaltime_label))
    {
        lv_label_set_text(guider_ui.music_totaltime_label, total_buf);
    }

    if (guider_ui.music_play_progress_slider && lv_obj_is_valid(guider_ui.music_play_progress_slider))
    {
        lv_slider_set_value(guider_ui.music_play_progress_slider, music_get_progress_percent(), LV_ANIM_OFF);
    }
}

static void music_render_lyric_canvas(void)
{
    if (!guider_ui.music_lyric_canvas || !lv_obj_is_valid(guider_ui.music_lyric_canvas))
        return;

    if (!guider_ui.music_lyric_cont || !lv_obj_is_valid(guider_ui.music_lyric_cont))
        return;

    if (lv_obj_has_flag(guider_ui.music_lyric_cont, LV_OBJ_FLAG_HIDDEN))
        return;

    // 设置歌词画布背景颜色
    lv_canvas_fill_bg(guider_ui.music_lyric_canvas, lv_color_hex(0xc3c1c1), LV_OPA_COVER);

    // 初始化歌词绘制描述符
    lv_draw_label_dsc_t normal_dsc;
    lv_draw_label_dsc_t current_dsc;
    lv_draw_label_dsc_t hint_dsc;
    lv_draw_label_dsc_init(&normal_dsc);
    lv_draw_label_dsc_init(&current_dsc);
    lv_draw_label_dsc_init(&hint_dsc);

    // 设置歌词绘制描述符字体、颜色和对齐方式
    normal_dsc.font = &lv_font_Fangzhengcuheisongsimple_14;
    normal_dsc.color = lv_color_hex(0x4d4d4d);
    normal_dsc.align = LV_TEXT_ALIGN_CENTER;

    // 设置当前歌词绘制描述符字体、颜色和对齐方式
    current_dsc.font = &lv_font_Fangzhengcuheisongsimple_14;
    current_dsc.color = lv_color_hex(0x0b8f3a);
    current_dsc.align = LV_TEXT_ALIGN_CENTER;

    // 设置提示歌词绘制描述符字体、颜色和对齐方式
    hint_dsc.font = &lv_font_Fangzhengcuheisongsimple_14;
    hint_dsc.color = lv_color_hex(0x666666);
    hint_dsc.align = LV_TEXT_ALIGN_CENTER;

    uint16_t lyric_count = music_get_lyric_count();
    int16_t current_index = music_get_current_lyric_index();

    if (lyric_count == 0)
    {
        lv_canvas_draw_text(guider_ui.music_lyric_canvas, 10, 72, 280, &hint_dsc, "未找到歌词");
        return;
    }

    if (current_index < 0)
        current_index = 0;
    if (current_index >= (int16_t)lyric_count)
        current_index = (int16_t)(lyric_count - 1);

    const char *prev_line = (current_index > 0) ? music_get_lyric_line((uint16_t)(current_index - 1)) : "";
    const char *curr_line = music_get_lyric_line((uint16_t)current_index);
    const char *next_line =
        ((uint16_t)(current_index + 1) < lyric_count) ? music_get_lyric_line((uint16_t)(current_index + 1)) : "";

    lv_canvas_draw_text(guider_ui.music_lyric_canvas, 10, 28, 280, &normal_dsc,
                        (prev_line && prev_line[0] != '\0') ? prev_line : " ");
    lv_canvas_draw_text(guider_ui.music_lyric_canvas, 10, 76, 280, &current_dsc,
                        (curr_line && curr_line[0] != '\0') ? curr_line : " ");
    lv_canvas_draw_text(guider_ui.music_lyric_canvas, 10, 124, 280, &normal_dsc,
                        (next_line && next_line[0] != '\0') ? next_line : " ");
}

static void music_ui_refresh_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    if (!guider_ui.music || !lv_obj_is_valid(guider_ui.music))
        return;

    uint32_t elapsed_ms = music_get_elapsed_time_ms();
    uint32_t total_ms = music_get_total_time_ms();
    uint8_t progress = music_get_progress_percent();
    int16_t lyric_index = music_get_current_lyric_index();

    bool progress_changed = (elapsed_ms != s_last_music_elapsed_ms) ||
                            (total_ms != s_last_music_total_ms) ||
                            (progress != s_last_music_progress);
    bool lyric_changed = (lyric_index != s_last_music_lyric_index);

    if (progress_changed) {
        music_update_progress_ui();
        s_last_music_elapsed_ms = elapsed_ms;
        s_last_music_total_ms = total_ms;
        s_last_music_progress = progress;
    }

    if (lyric_changed) {
        music_render_lyric_canvas();
        s_last_music_lyric_index = lyric_index;
    }
}

static void music_list_refresh(void)
{
    if (!guider_ui.music_music_list || !lv_obj_is_valid(guider_ui.music_music_list))
        return;

    /* 清空旧列表 */
    lv_obj_clean(guider_ui.music_music_list);

    s_music_count = music_get_count();
    if (s_music_count == 0)
    {
        lv_obj_t *btn = lv_list_add_btn(guider_ui.music_music_list, LV_SYMBOL_WARNING, "未找到WAV文件");
        if (btn)
        {
            style_list_btn(btn);
            lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        }
        return;
    }

    for (int i = 0; i < (int)s_music_count && i < MUSIC_MAX_FILES; i++)
    {
        const char *name = music_get_name(i);
        if (!name) continue;

        snprintf(s_music_name_cache[i], sizeof(s_music_name_cache[0]), "%s", name);

        lv_obj_t *btn = lv_list_add_btn(guider_ui.music_music_list, LV_SYMBOL_AUDIO, name);
        if (btn)
        {
            /* token = index + 1，0 保留为无效值 */
            lv_obj_set_user_data(btn, (void *)(uintptr_t)(i + 1));
            style_list_btn(btn);
            lv_obj_add_event_cb(btn, music_list_item_click_event_handler, LV_EVENT_CLICKED, &guider_ui);
        }
    }
}

/**
 * @brief 更新音乐播放状态显示
 */
static void music_update_play_status(void)
{
    if (!guider_ui.music || !lv_obj_is_valid(guider_ui.music))
        return;

    /* 当前歌曲名 */
    if (guider_ui.music_music_now_label && lv_obj_is_valid(guider_ui.music_music_now_label))
    {
        const char *name = music_get_current_name();
        lv_label_set_text(guider_ui.music_music_now_label, name ? name : "未播放");
    }

    /* 播放/暂停按钮状态 */
    if (guider_ui.music_play_control__imgbtn && lv_obj_is_valid(guider_ui.music_play_control__imgbtn))
    {
        bool playing = music_is_playing() && !music_is_paused();
        if (playing)
            lv_obj_add_state(guider_ui.music_play_control__imgbtn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(guider_ui.music_play_control__imgbtn, LV_STATE_CHECKED);
    }

    /* 音量数字 */
    if (guider_ui.music_volume_number_label && lv_obj_is_valid(guider_ui.music_volume_number_label))
    {
        lv_label_set_text_fmt(guider_ui.music_volume_number_label, "%d", music_get_volume());
    }

    /* 音量滑条 */
    if (guider_ui.music_volume_slider && lv_obj_is_valid(guider_ui.music_volume_slider))
    {
        lv_slider_set_value(guider_ui.music_volume_slider, music_get_volume(), LV_ANIM_OFF);
    }

    music_update_progress_ui();

    s_last_music_elapsed_ms = UINT32_MAX;
    s_last_music_total_ms = UINT32_MAX;
    s_last_music_progress = 0xFF;
    s_last_music_lyric_index = INT16_MIN;
}

/**
 * @brief 音乐事件处理函数
 * @param event_id 音乐事件ID
 */
static void music_event_handler(music_event_id_t event_id)
{
    if (event_id == MUSIC_EVT_PROGRESS) 
        return;

    if (!lvgl_port_lock(0)) 
        return;

    switch (event_id)
    {
        case MUSIC_EVT_STARTED:
            music_update_play_status();
            break;
        case MUSIC_EVT_PAUSED:
            if (guider_ui.music_play_control__imgbtn)
                lv_obj_clear_state(guider_ui.music_play_control__imgbtn, LV_STATE_CHECKED);
            break;
        case MUSIC_EVT_RESUMED:
            if (guider_ui.music_play_control__imgbtn)
                lv_obj_add_state(guider_ui.music_play_control__imgbtn, LV_STATE_CHECKED);
            break;
        case MUSIC_EVT_STOPPED:
            music_update_play_status();
            break;
        case MUSIC_EVT_SONG_END:
            music_update_play_status();
            break;
        case MUSIC_EVT_SCAN_DONE:
            music_list_refresh();
            break;
        case MUSIC_EVT_VOLUME_CHANGED:
            if (guider_ui.music_volume_number_label)
                lv_label_set_text_fmt(guider_ui.music_volume_number_label, "%d", music_get_volume());
            if (guider_ui.music_volume_slider)
                lv_slider_set_value(guider_ui.music_volume_slider, music_get_volume(), LV_ANIM_OFF);
            break;
        default:
            break;
    }
    lvgl_port_unlock();
}

/**
 * @brief 绑定音乐事件处理函数
 * @param ui LVGL UI句柄
 */
void bind_music_event_handler(lv_ui *ui)
{
    (void)ui;
    music_register_handler(music_event_handler);

    if (s_music_ui_timer == NULL) {
        s_music_ui_timer = lv_timer_create(music_ui_refresh_timer_cb, 300, NULL);
    }
}

/* ===================== END MUSIC UI更新 ==================== */

