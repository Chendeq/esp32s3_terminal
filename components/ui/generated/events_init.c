/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "wifi.h"
#include "lvgl_port.h"
#include "serial.h"
#include "esp_lvgl_port.h"
#include "music.h"
#include "mic.h"
#include "app_task.h"
#include "app_ui.h"
#include "xfyun.h"
#include "game.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


static void home_weather_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.weather, guider_ui.weather_del, &guider_ui.home_del, setup_scr_weather, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_serial_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.serial, guider_ui.serial_del, &guider_ui.home_del, setup_scr_serial, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_music_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.music, guider_ui.music_del, &guider_ui.home_del, setup_scr_music, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_setting_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.setting, guider_ui.setting_del, &guider_ui.home_del, setup_scr_setting, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_ai_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.ai1, guider_ui.ai1_del, &guider_ui.home_del, setup_scr_ai1, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_calendar_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.calendar, guider_ui.calendar_del, &guider_ui.home_del, setup_scr_calendar, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_game_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.gamechose, guider_ui.gamechose_del, &guider_ui.home_del, setup_scr_gamechose, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_about_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.about, guider_ui.about_del, &guider_ui.home_del, setup_scr_about, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void home_record_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.record, guider_ui.record_del, &guider_ui.home_del, setup_scr_record, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_home (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->home_weather_imgbtn, home_weather_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_serial_imgbtn, home_serial_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_music_imgbtn, home_music_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_setting_imgbtn, home_setting_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_ai_imgbtn, home_ai_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_calendar_imgbtn, home_calendar_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_game_imgbtn, home_game_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_about_imgbtn, home_about_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_record_imgbtn, home_record_imgbtn_event_handler, LV_EVENT_ALL, ui);
}

static void setting_connect_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        // 从密码输入框读取密码
        const char *pwd = lv_textarea_get_text(guider_ui.setting_pwd_ta);
        if (pwd) 
        {
            strncpy(wifi_req.password, pwd, sizeof(wifi_req.password) - 1);
            wifi_req.password[sizeof(wifi_req.password) - 1] = '\0';
        }
        wifi_send_cmd(WIFI_CMD_CONNECT);
        break;
    }
    default:
        break;
    }
}

static void setting_cancel_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.setting_pwd_cont, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void setting_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.setting_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void setting_light_slider_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        int brightness = lv_slider_get_value(lv_event_get_target(e));
        lv_label_set_text_fmt(guider_ui.setting_light_value_label, "%d%%", brightness);

        lv_port_backlight_init();
        lv_port_set_backlight_brightness(brightness);
        break;
    }
    default:
        break;
    }
}

static void setting_disconnect_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        wifi_send_cmd(WIFI_CMD_DISCONNECT);
        break;
    }
    default:
        break;
    }
}

static void setting_refresh_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        wifi_send_cmd(WIFI_CMD_REFRESH);
        break;
    }
    default:
        break;
    }
}

static void setting_auto_connect_sw_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * sw = lv_event_get_target(e);
        bool enabled = lv_obj_has_state(sw, LV_STATE_CHECKED);
        wifi_set_auto_connect_enabled(enabled);
        break;
    }
    default:
        break;
    }
}

void events_init_setting (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->setting_connect_btn, setting_connect_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_cancel_imgbtn, setting_cancel_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_back_imgbtn, setting_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_light_slider, setting_light_slider_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_disconnect_btn, setting_disconnect_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_refresh_btn, setting_refresh_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_auto_connect_sw, setting_auto_connect_sw_event_handler, LV_EVENT_ALL, ui);

    wifi_send_cmd(WIFI_CMD_REFRESH);
}

static void serial_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        serial_set_rx_active(false);
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.serial_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void serial_send_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        const char* text = lv_textarea_get_text(guider_ui.serial_tx_ta);
        size_t len = strlen(text);
        if (text != NULL && len > 0) {
            serial_port_write((const char *)text, len);
        }
        break;
    }
    default:
        break;
    }
}

static void serial_clear_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_textarea_set_text(guider_ui.serial_tx_ta, "");
        lv_textarea_set_text(guider_ui.serial_rx_ta, "");
        break;
    }
    default:
        break;
    }
}

static void serial_baud_ddlist_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.serial_baud_ddlist);
        uint32_t baudrate = 115200;
        switch (id) {
            case 0: baudrate = 115200; break;
            case 1: baudrate = 9600;   break;
            case 2: baudrate = 19200;  break;
            case 3: baudrate = 38400;  break;
            case 4: baudrate = 57600;  break;
            default: baudrate = 115200; break;
        }
        
        uart_set_baudrate(SERIAL_UART_PORT, baudrate);
        break;
    }
    default:
        break;
    }
}

void events_init_serial (lv_ui *ui)
{
    serial_set_rx_active(true);
    lv_obj_add_event_cb(ui->serial_back_imgbtn, serial_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->serial_send_imgbtn, serial_send_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->serial_clear_imgbtn, serial_clear_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->serial_baud_ddlist, serial_baud_ddlist_event_handler, LV_EVENT_ALL, ui);
}

static void weather_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.weather_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_weather (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->weather_back_imgbtn, weather_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
}

static void music_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_task_stop();
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.music_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void music_volume_slider_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t *slider = lv_event_get_target(e);
        int value = lv_slider_get_value(slider);
        music_cmd_msg_t msg = { .cmd = MUSIC_CMD_VOLUME, .data.volume = (uint8_t)value };
        music_send_cmd(&msg);
        break;
    }
    default:
        break;
    }
}

static void music_play_control__imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_cmd_msg_t msg = { .cmd = MUSIC_CMD_PLAY_PAUSE };
        music_send_cmd(&msg);
        break;
    }
    default:
        break;
    }
}

static void music_previous_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_cmd_msg_t msg = { .cmd = MUSIC_CMD_PREV };
        music_send_cmd(&msg);
        break;
    }
    default:
        break;
    }
}

static void music_next_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_cmd_msg_t msg = { .cmd = MUSIC_CMD_NEXT };
        music_send_cmd(&msg);
        break;
    }
    default:
        break;
    }
}

static void music_lyric_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.music_lyric_cont, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void music_lyric_cancel_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.music_lyric_cont, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void music_screen_load_cb(lv_event_t *e)
{
    (void)e;
    /* 启动音乐解码任务 */
    music_task_start();
    music_cmd_msg_t msg = { .cmd = MUSIC_CMD_SCAN };
    music_send_cmd(&msg);
}

static void music_screen_unload_cb(lv_event_t *e)
{
    (void)e;
    /* 停止解码任务（任务常驻，仅退出播放循环） */
    music_task_stop();
}

void events_init_music (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->music_back_imgbtn, music_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_volume_slider, music_volume_slider_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_play_control__imgbtn, music_play_control__imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_previous_imgbtn, music_previous_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_next_imgbtn, music_next_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_lyric_imgbtn, music_lyric_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_lyric_cancel_imgbtn, music_lyric_cancel_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music, music_screen_load_cb, LV_EVENT_SCREEN_LOADED, ui);
    lv_obj_add_event_cb(ui->music, music_screen_unload_cb, LV_EVENT_SCREEN_UNLOAD_START, ui);
}

static void ai1_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.ai1_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void ai1_send_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        const char *question = lv_textarea_get_text(guider_ui.ai1_aitx_ta);
        if (!question || question[0] == '\0')
            return;

        esp_err_t ret = xfyun_ai_send_question(question);
        if (ret != ESP_OK) {
            ai_ui_set_answer(ret == ESP_ERR_TIMEOUT ? "Busy" : "Send failed");
        }
        break;
    }
    default:
        break;
    }
}

static void ai1_clear_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_textarea_set_text(guider_ui.ai1_airx_ta, "");
        lv_textarea_set_cursor_pos(guider_ui.ai1_airx_ta, LV_TEXTAREA_CURSOR_LAST);
        break;
    }
    default:
        break;
    }
}

void events_init_ai1 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->ai1_back_imgbtn, ai1_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->ai1_send_imgbtn, ai1_send_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->ai1_clear_imgbtn, ai1_clear_imgbtn_event_handler, LV_EVENT_ALL, ui);
}

static void calendar_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.calendar_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_calendar (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->calendar_back_imgbtn, calendar_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
}

static void gamechose_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.gamechose_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void gamechose_btn_2048_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        game_select(GAME_TYPE_2048);
        ui_load_scr_animation(&guider_ui, &guider_ui.gameplay, guider_ui.gameplay_del, &guider_ui.gamechose_del, setup_scr_gameplay, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void gamechose_yang_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        game_select(GAME_TYPE_YANG);
        ui_load_scr_animation(&guider_ui, &guider_ui.gameplay, guider_ui.gameplay_del, &guider_ui.gamechose_del, setup_scr_gameplay, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void gamechose_zhi_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        game_select(GAME_TYPE_PVZ);
        ui_load_scr_animation(&guider_ui, &guider_ui.gameplay, guider_ui.gameplay_del, &guider_ui.gamechose_del, setup_scr_gameplay, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void gamechose_xiao_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        game_select(GAME_TYPE_XIAOXIAOLE);
        ui_load_scr_animation(&guider_ui, &guider_ui.gameplay, guider_ui.gameplay_del, &guider_ui.gamechose_del, setup_scr_gameplay, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_gamechose (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->gamechose_back_imgbtn, gamechose_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->gamechose_btn_2048, gamechose_btn_2048_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->gamechose_yang_btn, gamechose_yang_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->gamechose_zhi_btn, gamechose_zhi_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->gamechose_xiao_btn, gamechose_xiao_btn_event_handler, LV_EVENT_ALL, ui);
}

static void gameplay_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        game_stop_current();
        ui_load_scr_animation(&guider_ui, &guider_ui.gamechose, guider_ui.gamechose_del, &guider_ui.gameplay_del, setup_scr_gamechose, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_gameplay (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->gameplay_back_imgbtn, gameplay_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
}

static void about_back_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.about_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_about (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->about_back_imgbtn, about_back_imgbtn_event_handler, LV_EVENT_ALL, ui);
}

static void record_update_buttons(lv_ui *ui)
{
    mic_record_status_t status;
    if (!mic_get_record_status(&status)) {
        return;
    }
    bool can_stop = status.state == MIC_RECORD_STARTING || status.state == MIC_RECORDING;
    bool busy = can_stop || status.state == MIC_RECORD_STOPPING;
    if (busy) {
        lv_obj_add_state(ui->record_rec_start_btn, LV_STATE_DISABLED);
    } else {
        lv_obj_clear_state(ui->record_rec_start_btn, LV_STATE_DISABLED);
    }
    if (can_stop) {
        lv_obj_clear_state(ui->record_rec_stop_btn, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(ui->record_rec_stop_btn, LV_STATE_DISABLED);
    }
    for (uint32_t i = 0; i < lv_obj_get_child_cnt(ui->record_record_list); ++i) {
        lv_obj_t *item = lv_obj_get_child(ui->record_record_list, i);
        if (busy) {
            lv_obj_add_state(item, LV_STATE_DISABLED);
        } else {
            lv_obj_clear_state(item, LV_STATE_DISABLED);
        }
    }
}

void record_list_item_event_handler(lv_event_t *e)
{
    record_ui_play_file(lv_event_get_user_data(e), lv_event_get_target(e));
}

static void record_buttons_timer_cb(lv_timer_t *timer)
{
    record_update_buttons(timer->user_data);
}

static void record_buttons_delete_cb(lv_event_t *e)
{
    lv_timer_del(lv_event_get_user_data(e));
}

static void record_record_imgbtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        mic_record_stop();
        music_task_stop();
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.record_del, setup_scr_home, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void record_rec_start_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_cmd_msg_t msg = { .cmd = MUSIC_CMD_STOP };
        if (g_music_cmd_queue == NULL || music_send_cmd(&msg) == pdTRUE) {
            mic_record_start();
        }
        record_update_buttons(lv_event_get_user_data(e));
        break;
    }
    default:
        break;
    }
}

static void record_rec_stop_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        mic_record_stop();
        record_update_buttons(lv_event_get_user_data(e));
        break;
    }
    default:
        break;
    }
}

void events_init_record (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->record_record_imgbtn, record_record_imgbtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->record_rec_start_btn, record_rec_start_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->record_rec_stop_btn, record_rec_stop_btn_event_handler, LV_EVENT_ALL, ui);
    record_update_buttons(ui);
    lv_timer_t *timer = lv_timer_create(record_buttons_timer_cb, 100, ui);
    if (timer != NULL) {
        lv_obj_add_event_cb(ui->record, record_buttons_delete_cb, LV_EVENT_DELETE, timer);
    }
    bind_mic_event_handler(ui);
}


void events_init(lv_ui *ui)
{

}
