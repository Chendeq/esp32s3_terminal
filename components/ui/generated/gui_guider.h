/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *home;
	bool home_del;
	lv_obj_t *home_wifi_img;
	lv_obj_t *home_welcome_label;
	lv_obj_t *home_time_label;
	lv_obj_t *home_date_label;
	lv_obj_t *home_week_label;
	lv_obj_t *home_tileview_1;
	lv_obj_t *home_tileview_1_tile_1;
	lv_obj_t *home_tileview_1_tile_2;
	lv_obj_t *home_tileview_1_tile_3;
	lv_obj_t *home_weather_imgbtn;
	lv_obj_t *home_weather_imgbtn_label;
	lv_obj_t *home_serial_imgbtn;
	lv_obj_t *home_serial_imgbtn_label;
	lv_obj_t *home_music_imgbtn;
	lv_obj_t *home_music_imgbtn_label;
	lv_obj_t *home_setting_imgbtn;
	lv_obj_t *home_setting_imgbtn_label;
	lv_obj_t *home_setting_label;
	lv_obj_t *home_serial_label;
	lv_obj_t *home_weather_label;
	lv_obj_t *home_music_label;
	lv_obj_t *home_ai_label;
	lv_obj_t *home_ai_imgbtn;
	lv_obj_t *home_ai_imgbtn_label;
	lv_obj_t *home_calendar_imgbtn;
	lv_obj_t *home_calendar_imgbtn_label;
	lv_obj_t *home_game_imgbtn;
	lv_obj_t *home_game_imgbtn_label;
	lv_obj_t *home_about_imgbtn;
	lv_obj_t *home_about_imgbtn_label;
	lv_obj_t *home_calendar_label;
	lv_obj_t *home_game_label;
	lv_obj_t *home_about_label;
	lv_obj_t *home_record_label;
	lv_obj_t *home_record_imgbtn;
	lv_obj_t *home_record_imgbtn_label;
	lv_obj_t *home_today_weather_label;
	lv_obj_t *home_weather_status_label;
	lv_obj_t *home_home_address_label;
	lv_obj_t *setting;
	bool setting_del;
	lv_obj_t *setting_pwd_cont;
	lv_obj_t *setting_wifi_id_label;
	lv_obj_t *setting_pwd_ta;
	lv_obj_t *setting_connect_btn;
	lv_obj_t *setting_connect_btn_label;
	lv_obj_t *setting_cancel_imgbtn;
	lv_obj_t *setting_cancel_imgbtn_label;
	lv_obj_t *setting_back_imgbtn;
	lv_obj_t *setting_back_imgbtn_label;
	lv_obj_t *setting_light_slider;
	lv_obj_t *setting_light_set_label;
	lv_obj_t *setting_light_value_label;
	lv_obj_t *setting_wifi_label;
	lv_obj_t *setting_wifi_img;
	lv_obj_t *setting_wifi_ssid_label;
	lv_obj_t *setting_disconnect_btn;
	lv_obj_t *setting_disconnect_btn_label;
	lv_obj_t *setting_wifi_list;
	lv_obj_t *setting_refresh_btn;
	lv_obj_t *setting_refresh_btn_label;
	lv_obj_t *setting_auto_connect_sw;
	lv_obj_t *setting_auto_connect_label;
	lv_obj_t *serial;
	bool serial_del;
	lv_obj_t *serial_back_imgbtn;
	lv_obj_t *serial_back_imgbtn_label;
	lv_obj_t *serial_tx_ta;
	lv_obj_t *serial_rx_ta;
	lv_obj_t *serial_send_imgbtn;
	lv_obj_t *serial_send_imgbtn_label;
	lv_obj_t *serial_clear_imgbtn;
	lv_obj_t *serial_clear_imgbtn_label;
	lv_obj_t *serial_baud_ddlist;
	lv_obj_t *weather;
	bool weather_del;
	lv_obj_t *weather_back_imgbtn;
	lv_obj_t *weather_back_imgbtn_label;
	lv_obj_t *weather_last_update_label;
	lv_obj_t *weather_last_time_label;
	lv_obj_t *weather_address_label;
	lv_obj_t *weather_address_img;
	lv_obj_t *weather_weather_img_1;
	lv_obj_t *weather_weather_img_2;
	lv_obj_t *weather_weather_img_3;
	lv_obj_t *weather_date_label_1;
	lv_obj_t *weather_date_label_2;
	lv_obj_t *weather_date_label_3;
	lv_obj_t *weather_status_label_1;
	lv_obj_t *weather_status_label_2;
	lv_obj_t *weather_status_label_3;
	lv_obj_t *weather_temp_label_1;
	lv_obj_t *weather_temp_label_2;
	lv_obj_t *weather_temp_label_3;
	lv_obj_t *music;
	bool music_del;
	lv_obj_t *music_back_imgbtn;
	lv_obj_t *music_back_imgbtn_label;
	lv_obj_t *music_volume_slider;
	lv_obj_t *music_volume_label;
	lv_obj_t *music_music_now_label;
	lv_obj_t *music_music_list;
	lv_obj_t *music_play_control__imgbtn;
	lv_obj_t *music_play_control__imgbtn_label;
	lv_obj_t *music_previous_imgbtn;
	lv_obj_t *music_previous_imgbtn_label;
	lv_obj_t *music_next_imgbtn;
	lv_obj_t *music_next_imgbtn_label;
	lv_obj_t *music_volume_number_label;
	lv_obj_t *music_lyric_imgbtn;
	lv_obj_t *music_lyric_imgbtn_label;
	lv_obj_t *music_lyric_cont;
	lv_obj_t *music_lyric_canvas;
	lv_obj_t *music_lyric_cancel_imgbtn;
	lv_obj_t *music_lyric_cancel_imgbtn_label;
	lv_obj_t *music_totaltime_label;
	lv_obj_t *music_time_play_label;
	lv_obj_t *music_play_progress_slider;
	lv_obj_t *ai1;
	bool ai1_del;
	lv_obj_t *ai1_back_imgbtn;
	lv_obj_t *ai1_back_imgbtn_label;
	lv_obj_t *ai1_aitx_ta;
	lv_obj_t *ai1_airx_ta;
	lv_obj_t *ai1_send_imgbtn;
	lv_obj_t *ai1_send_imgbtn_label;
	lv_obj_t *ai1_clear_imgbtn;
	lv_obj_t *ai1_clear_imgbtn_label;
	lv_obj_t *ai1_aibq_img;
	lv_obj_t *ai1_user_img;
	lv_obj_t *calendar;
	bool calendar_del;
	lv_obj_t *calendar_calendar_1;
	lv_obj_t *calendar_back_imgbtn;
	lv_obj_t *calendar_back_imgbtn_label;
	lv_obj_t *gamechose;
	bool gamechose_del;
	lv_obj_t *gamechose_back_imgbtn;
	lv_obj_t *gamechose_back_imgbtn_label;
	lv_obj_t *gamechose_btn_2048;
	lv_obj_t *gamechose_btn_2048_label;
	lv_obj_t *gamechose_yang_btn;
	lv_obj_t *gamechose_yang_btn_label;
	lv_obj_t *gamechose_zhi_btn;
	lv_obj_t *gamechose_zhi_btn_label;
	lv_obj_t *gamechose_xiao_btn;
	lv_obj_t *gamechose_xiao_btn_label;
	lv_obj_t *gameplay;
	bool gameplay_del;
	lv_obj_t *gameplay_back_imgbtn;
	lv_obj_t *gameplay_back_imgbtn_label;
	lv_obj_t *about;
	bool about_del;
	lv_obj_t *about_back_imgbtn;
	lv_obj_t *about_back_imgbtn_label;
	lv_obj_t *about_github_img;
	lv_obj_t *about_github_label;
	lv_obj_t *about_espressif_img;
	lv_obj_t *about_kfzc_label;
	lv_obj_t *about_vscode_img;
	lv_obj_t *about_label_1;
	lv_obj_t *about_label_2;
	lv_obj_t *about_xunfei_img;
	lv_obj_t *about_label_3;
	lv_obj_t *about_guider_img;
	lv_obj_t *about_lib_label;
	lv_obj_t *about_platform_label;
	lv_obj_t *about_casnhu_label;
	lv_obj_t *about_flash_label;
	lv_obj_t *about_psram_label;
	lv_obj_t *about_isram_label;
	lv_obj_t *about_lcd_label;
	lv_obj_t *about_display_label;
	lv_obj_t *about_touch_label;
	lv_obj_t *about_chip_label;
	lv_obj_t *about_chip_img;
	lv_obj_t *about_reference_label;
	lv_obj_t *record;
	bool record_del;
	lv_obj_t *record_record_imgbtn;
	lv_obj_t *record_record_imgbtn_label;
	lv_obj_t *record_rec_start_btn;
	lv_obj_t *record_rec_start_btn_label;
	lv_obj_t *record_rec_stop_btn;
	lv_obj_t *record_rec_stop_btn_label;
	lv_obj_t *record_duration_label;
	lv_obj_t *record_record_list;
	lv_obj_t *record_record_list_item0;
	lv_obj_t *record_rec_state_label;
	lv_obj_t *record_msg_label;
	lv_obj_t *g_kb_top_layer;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_home(lv_ui *ui);
void setup_scr_setting(lv_ui *ui);
void setup_scr_serial(lv_ui *ui);
void setup_scr_weather(lv_ui *ui);
void setup_scr_music(lv_ui *ui);
void setup_scr_ai1(lv_ui *ui);
void setup_scr_calendar(lv_ui *ui);
void setup_scr_gamechose(lv_ui *ui);
void setup_scr_gameplay(lv_ui *ui);
void setup_scr_about(lv_ui *ui);
void setup_scr_record(lv_ui *ui);
LV_IMG_DECLARE(__wifi_16x16_alpha_16x16);
LV_IMG_DECLARE(_weather_alpha_64x64);
LV_IMG_DECLARE(_serial_alpha_64x64);
LV_IMG_DECLARE(_music_alpha_64x64);
LV_IMG_DECLARE(_setting_alpha_64x64);
LV_IMG_DECLARE(_ai_alpha_64x64);
LV_IMG_DECLARE(_calendar_alpha_64x64);
LV_IMG_DECLARE(_game_alpha_64x64);
LV_IMG_DECLARE(_about_alpha_64x64);
LV_IMG_DECLARE(_mic_alpha_64x64);
LV_IMG_DECLARE(_cancel_alpha_20x20);
LV_IMG_DECLARE(_cancel_press_alpha_20x20);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_wifi_is_connect_alpha_16x16);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_send_alpha_32x32);
LV_IMG_DECLARE(_send_p_alpha_32x32);
LV_IMG_DECLARE(_clear_alpha_32x32);
LV_IMG_DECLARE(_clear_p_alpha_32x32);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_address_alpha_20x20);
LV_IMG_DECLARE(_sun_alpha_48x48);
LV_IMG_DECLARE(_cloudy_alpha_48x48);
LV_IMG_DECLARE(_rain_alpha_48x48);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_play_alpha_32x32);
LV_IMG_DECLARE(_pause_alpha_32x32);
LV_IMG_DECLARE(_previous_song_alpha_32x32);
LV_IMG_DECLARE(_previous_press_alpha_32x32);
LV_IMG_DECLARE(_next_song_alpha_32x32);
LV_IMG_DECLARE(_no_wifi_alpha_16x16);
LV_IMG_DECLARE(_next_press_alpha_32x32);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_send_alpha_32x32);
LV_IMG_DECLARE(_send_p_alpha_32x32);
LV_IMG_DECLARE(_clear_alpha_32x32);
LV_IMG_DECLARE(_clear_p_alpha_32x32);
LV_IMG_DECLARE(_aibq_alpha_32x32);
LV_IMG_DECLARE(_user_alpha_32x32);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_back_release_alpha_28x28);
LV_IMG_DECLARE(_back_press_alpha_28x28);
LV_IMG_DECLARE(_github_alpha_32x32);
LV_IMG_DECLARE(_lex_alpha_32x32);
LV_IMG_DECLARE(_Vscode_alpha_32x32);
LV_IMG_DECLARE(_xunfei_alpha_32x32);
LV_IMG_DECLARE(_gui_guider_alpha_32x32);
LV_IMG_DECLARE(_chip_alpha_32x32);
LV_IMG_DECLARE(_lyric_alpha_32x32);

LV_FONT_DECLARE(lv_font_Fangzhengcuheisongsimple_16)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Fangzhengcuheisongsimple_50)
LV_FONT_DECLARE(lv_font_Fangzhengcuheisongsimple_20)
LV_FONT_DECLARE(lv_font_Fangzhengcuheisongsimple_14)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_14)
LV_FONT_DECLARE(lv_font_Fangzhengcuheisongsimple_12)


#ifdef __cplusplus
}
#endif
#endif
