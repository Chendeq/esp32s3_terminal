/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"
#include "wifi.h"

void setup_scr_setting(lv_ui *ui)
{
    //Write codes setting
    ui->setting = lv_obj_create(NULL);
    lv_obj_set_size(ui->setting, 320, 240);
    lv_obj_set_scrollbar_mode(ui->setting, LV_SCROLLBAR_MODE_OFF);

    //Write style for setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_pwd_cont
    ui->setting_pwd_cont = lv_obj_create(ui->setting);
    lv_obj_set_pos(ui->setting_pwd_cont, 60, 10);
    lv_obj_set_size(ui->setting_pwd_cont, 210, 100);
    lv_obj_set_scrollbar_mode(ui->setting_pwd_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->setting_pwd_cont, LV_OBJ_FLAG_HIDDEN);

    //Write style for setting_pwd_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_pwd_cont, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->setting_pwd_cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->setting_pwd_cont, lv_color_hex(0xa6bed2), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->setting_pwd_cont, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_pwd_cont, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_pwd_cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_pwd_cont, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_pwd_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_pwd_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_pwd_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_pwd_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_pwd_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_pwd_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_wifi_id_label
    ui->setting_wifi_id_label = lv_label_create(ui->setting_pwd_cont);
    lv_label_set_text(ui->setting_wifi_id_label, "IP");
    lv_label_set_long_mode(ui->setting_wifi_id_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_pos(ui->setting_wifi_id_label, 50, 10);
    lv_obj_set_size(ui->setting_wifi_id_label, 100, 15);

    //Write style for setting_wifi_id_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_wifi_id_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_wifi_id_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_wifi_id_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_wifi_id_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_wifi_id_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_pwd_ta
    ui->setting_pwd_ta = lv_textarea_create(ui->setting_pwd_cont);
    lv_textarea_set_text(ui->setting_pwd_ta, "");
    lv_textarea_set_placeholder_text(ui->setting_pwd_ta, "password");
    lv_textarea_set_password_bullet(ui->setting_pwd_ta, "*");
    lv_textarea_set_password_mode(ui->setting_pwd_ta, false);
    lv_textarea_set_one_line(ui->setting_pwd_ta, false);
    lv_textarea_set_accepted_chars(ui->setting_pwd_ta, "");
    lv_textarea_set_max_length(ui->setting_pwd_ta, 50);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->setting_pwd_ta, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->setting_pwd_ta, 25, 35);
    lv_obj_set_size(ui->setting_pwd_ta, 160, 25);

    //Write style for setting_pwd_ta, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->setting_pwd_ta, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_pwd_ta, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_pwd_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_pwd_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_pwd_ta, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_pwd_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_pwd_ta, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_pwd_ta, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_pwd_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->setting_pwd_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->setting_pwd_ta, lv_color_hex(0xf2eeee), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->setting_pwd_ta, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_pwd_ta, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_pwd_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_pwd_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_pwd_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_pwd_ta, 5, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_pwd_ta, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_pwd_ta, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_pwd_ta, lv_color_hex(0xffffff), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_pwd_ta, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_pwd_ta, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes setting_connect_btn
    ui->setting_connect_btn = lv_btn_create(ui->setting_pwd_cont);
    ui->setting_connect_btn_label = lv_label_create(ui->setting_connect_btn);
    lv_label_set_text(ui->setting_connect_btn_label, "连接");
    lv_label_set_long_mode(ui->setting_connect_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_connect_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_connect_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->setting_connect_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->setting_connect_btn, 80, 70);
    lv_obj_set_size(ui->setting_connect_btn, 50, 20);

    //Write style for setting_connect_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_connect_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_connect_btn, lv_color_hex(0xe8ebc6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_connect_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_connect_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_connect_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_connect_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_connect_btn, lv_color_hex(0xff6500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_connect_btn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_connect_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_connect_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_connect_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_bg_opa(ui->setting_connect_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui->setting_connect_btn, lv_color_hex(0xd4d3d3), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(ui->setting_connect_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_border_width(ui->setting_connect_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_radius(ui->setting_connect_btn, 5, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->setting_connect_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->setting_connect_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->setting_connect_btn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->setting_connect_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes setting_cancel_imgbtn
    ui->setting_cancel_imgbtn = lv_imgbtn_create(ui->setting_pwd_cont);
    lv_imgbtn_set_src(ui->setting_cancel_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_cancel_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->setting_cancel_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_cancel_press_alpha_20x20, NULL);
    ui->setting_cancel_imgbtn_label = lv_label_create(ui->setting_cancel_imgbtn);
    lv_label_set_text(ui->setting_cancel_imgbtn_label, "");
    lv_label_set_long_mode(ui->setting_cancel_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_cancel_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_cancel_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->setting_cancel_imgbtn, 5, 5);
    lv_obj_set_size(ui->setting_cancel_imgbtn, 20, 20);

    //Write style for setting_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->setting_cancel_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_cancel_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_cancel_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->setting_cancel_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->setting_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->setting_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->setting_cancel_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->setting_cancel_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->setting_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->setting_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for setting_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->setting_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->setting_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->setting_cancel_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->setting_cancel_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->setting_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->setting_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for setting_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->setting_cancel_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->setting_cancel_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes setting_back_imgbtn
    ui->setting_back_imgbtn = lv_imgbtn_create(ui->setting);
    lv_imgbtn_set_src(ui->setting_back_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->setting_back_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->setting_back_imgbtn_label = lv_label_create(ui->setting_back_imgbtn);
    lv_label_set_text(ui->setting_back_imgbtn_label, "");
    lv_label_set_long_mode(ui->setting_back_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_back_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_back_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->setting_back_imgbtn, 5, 5);
    lv_obj_set_size(ui->setting_back_imgbtn, 28, 28);

    //Write style for setting_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->setting_back_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_back_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_back_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->setting_back_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->setting_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->setting_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->setting_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->setting_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->setting_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->setting_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for setting_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->setting_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->setting_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->setting_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->setting_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->setting_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->setting_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for setting_back_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->setting_back_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->setting_back_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes setting_light_slider
    ui->setting_light_slider = lv_slider_create(ui->setting);
    lv_slider_set_range(ui->setting_light_slider, 0, 100);
    lv_slider_set_mode(ui->setting_light_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->setting_light_slider, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->setting_light_slider, 110, 21);
    lv_obj_set_size(ui->setting_light_slider, 160, 10);

    //Write style for setting_light_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_light_slider, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_light_slider, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_light_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_light_slider, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->setting_light_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_light_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_light_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_light_slider, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_light_slider, lv_color_hex(0xf28125), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_light_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_light_slider, 5, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for setting_light_slider, Part: LV_PART_INDICATOR, State: LV_STATE_FOCUSED.
    lv_obj_set_style_bg_opa(ui->setting_light_slider, 255, LV_PART_INDICATOR|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->setting_light_slider, lv_color_hex(0xffb788), LV_PART_INDICATOR|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->setting_light_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->setting_light_slider, 8, LV_PART_INDICATOR|LV_STATE_FOCUSED);

    //Write style for setting_light_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_light_slider, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_light_slider, lv_color_hex(0x56ff06), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_light_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_light_slider, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes setting_light_set_label
    ui->setting_light_set_label = lv_label_create(ui->setting);
    lv_label_set_text(ui->setting_light_set_label, "亮度设置");
    lv_label_set_long_mode(ui->setting_light_set_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->setting_light_set_label, 40, 21);
    lv_obj_set_size(ui->setting_light_set_label, 60, 15);

    //Write style for setting_light_set_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_light_set_label, lv_color_hex(0xf28c25), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_light_set_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_light_set_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_light_set_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_light_set_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_light_value_label
    ui->setting_light_value_label = lv_label_create(ui->setting);
    lv_label_set_text(ui->setting_light_value_label, "100%");
    lv_label_set_long_mode(ui->setting_light_value_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->setting_light_value_label, 280, 21);
    lv_obj_set_size(ui->setting_light_value_label, 40, 15);

    //Write style for setting_light_value_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_light_value_label, lv_color_hex(0xff7a00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_light_value_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_light_value_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_light_value_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_light_value_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_wifi_label
    ui->setting_wifi_label = lv_label_create(ui->setting);
    lv_label_set_text(ui->setting_wifi_label, "WiFi连接");
    lv_label_set_long_mode(ui->setting_wifi_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->setting_wifi_label, 30, 55);
    lv_obj_set_size(ui->setting_wifi_label, 65, 15);

    //Write style for setting_wifi_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_wifi_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_wifi_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_wifi_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_wifi_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_wifi_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_wifi_img
    ui->setting_wifi_img = lv_img_create(ui->setting);
    lv_obj_add_flag(ui->setting_wifi_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->setting_wifi_img, &_wifi_is_connect_alpha_16x16);
    lv_img_set_pivot(ui->setting_wifi_img, 50,50);
    lv_img_set_angle(ui->setting_wifi_img, 0);
    lv_obj_set_pos(ui->setting_wifi_img, 110, 55);
    lv_obj_set_size(ui->setting_wifi_img, 16, 16);

    //Write style for setting_wifi_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->setting_wifi_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->setting_wifi_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_wifi_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->setting_wifi_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_wifi_ssid_label
    ui->setting_wifi_ssid_label = lv_label_create(ui->setting);
    lv_label_set_text(ui->setting_wifi_ssid_label, "未连接");
    lv_label_set_long_mode(ui->setting_wifi_ssid_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_pos(ui->setting_wifi_ssid_label, 140, 55);
    lv_obj_set_size(ui->setting_wifi_ssid_label, 80, 15);

    //Write style for setting_wifi_ssid_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_wifi_ssid_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_wifi_ssid_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_wifi_ssid_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_wifi_ssid_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_wifi_ssid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_disconnect_btn
    ui->setting_disconnect_btn = lv_btn_create(ui->setting);
    ui->setting_disconnect_btn_label = lv_label_create(ui->setting_disconnect_btn);
    lv_label_set_text(ui->setting_disconnect_btn_label, "断开连接");
    lv_label_set_long_mode(ui->setting_disconnect_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_disconnect_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_disconnect_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->setting_disconnect_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->setting_disconnect_btn, 135, 80);
    lv_obj_set_size(ui->setting_disconnect_btn, 60, 20);

    //Write style for setting_disconnect_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_disconnect_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_disconnect_btn, lv_color_hex(0xfad282), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_disconnect_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_disconnect_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_disconnect_btn, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_disconnect_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_disconnect_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_disconnect_btn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_disconnect_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_disconnect_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_disconnect_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_bg_opa(ui->setting_disconnect_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui->setting_disconnect_btn, lv_color_hex(0x8aabc6), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(ui->setting_disconnect_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_border_width(ui->setting_disconnect_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_radius(ui->setting_disconnect_btn, 5, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->setting_disconnect_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->setting_disconnect_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->setting_disconnect_btn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->setting_disconnect_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes setting_wifi_list
    ui->setting_wifi_list = lv_list_create(ui->setting);
    lv_obj_set_pos(ui->setting_wifi_list, 20, 110);
    lv_obj_set_size(ui->setting_wifi_list, 280, 120);
    lv_obj_set_scrollbar_mode(ui->setting_wifi_list, LV_SCROLLBAR_MODE_ON);

    //Write style state: LV_STATE_DEFAULT for &style_setting_wifi_list_main_main_default
    static lv_style_t style_setting_wifi_list_main_main_default;
    ui_init_style(&style_setting_wifi_list_main_main_default);

    lv_style_set_pad_top(&style_setting_wifi_list_main_main_default, 5);
    lv_style_set_pad_left(&style_setting_wifi_list_main_main_default, 5);
    lv_style_set_pad_right(&style_setting_wifi_list_main_main_default, 5);
    lv_style_set_pad_bottom(&style_setting_wifi_list_main_main_default, 5);
    lv_style_set_bg_opa(&style_setting_wifi_list_main_main_default, 255);
    lv_style_set_bg_color(&style_setting_wifi_list_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_setting_wifi_list_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_setting_wifi_list_main_main_default, 2);
    lv_style_set_border_opa(&style_setting_wifi_list_main_main_default, 255);
    lv_style_set_border_color(&style_setting_wifi_list_main_main_default, lv_color_hex(0xededee));
    lv_style_set_border_side(&style_setting_wifi_list_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_setting_wifi_list_main_main_default, 5);
    lv_style_set_shadow_width(&style_setting_wifi_list_main_main_default, 0);
    lv_obj_add_style(ui->setting_wifi_list, &style_setting_wifi_list_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_setting_wifi_list_main_scrollbar_default
    static lv_style_t style_setting_wifi_list_main_scrollbar_default;
    ui_init_style(&style_setting_wifi_list_main_scrollbar_default);

    lv_style_set_radius(&style_setting_wifi_list_main_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_setting_wifi_list_main_scrollbar_default, 233);
    lv_style_set_bg_color(&style_setting_wifi_list_main_scrollbar_default, lv_color_hex(0xfefefe));
    lv_style_set_bg_grad_dir(&style_setting_wifi_list_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->setting_wifi_list, &style_setting_wifi_list_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_setting_wifi_list_extra_btns_main_default
    static lv_style_t style_setting_wifi_list_extra_btns_main_default;
    ui_init_style(&style_setting_wifi_list_extra_btns_main_default);

    lv_style_set_pad_top(&style_setting_wifi_list_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_setting_wifi_list_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_setting_wifi_list_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_setting_wifi_list_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_setting_wifi_list_extra_btns_main_default, 2);
    lv_style_set_border_opa(&style_setting_wifi_list_extra_btns_main_default, 255);
    lv_style_set_border_color(&style_setting_wifi_list_extra_btns_main_default, lv_color_hex(0xe9eaea));
    lv_style_set_border_side(&style_setting_wifi_list_extra_btns_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_setting_wifi_list_extra_btns_main_default, lv_color_hex(0x000000));
    lv_style_set_text_font(&style_setting_wifi_list_extra_btns_main_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_setting_wifi_list_extra_btns_main_default, 255);
    lv_style_set_radius(&style_setting_wifi_list_extra_btns_main_default, 5);
    lv_style_set_bg_opa(&style_setting_wifi_list_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_setting_wifi_list_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_setting_wifi_list_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_setting_wifi_list_extra_texts_main_default
    static lv_style_t style_setting_wifi_list_extra_texts_main_default;
    ui_init_style(&style_setting_wifi_list_extra_texts_main_default);

    lv_style_set_pad_top(&style_setting_wifi_list_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_setting_wifi_list_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_setting_wifi_list_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_setting_wifi_list_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_setting_wifi_list_extra_texts_main_default, 2);
    lv_style_set_border_opa(&style_setting_wifi_list_extra_texts_main_default, 255);
    lv_style_set_border_color(&style_setting_wifi_list_extra_texts_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_setting_wifi_list_extra_texts_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_setting_wifi_list_extra_texts_main_default, lv_color_hex(0x000000));
    lv_style_set_text_font(&style_setting_wifi_list_extra_texts_main_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_setting_wifi_list_extra_texts_main_default, 255);
    lv_style_set_radius(&style_setting_wifi_list_extra_texts_main_default, 5);
    lv_style_set_transform_width(&style_setting_wifi_list_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_setting_wifi_list_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_setting_wifi_list_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_setting_wifi_list_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes setting_refresh_btn
    ui->setting_refresh_btn = lv_btn_create(ui->setting);
    ui->setting_refresh_btn_label = lv_label_create(ui->setting_refresh_btn);
    lv_label_set_text(ui->setting_refresh_btn_label, "刷新");
    lv_label_set_long_mode(ui->setting_refresh_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_refresh_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_refresh_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->setting_refresh_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->setting_refresh_btn, 20, 80);
    lv_obj_set_size(ui->setting_refresh_btn, 60, 20);

    //Write style for setting_refresh_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_refresh_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_refresh_btn, lv_color_hex(0xfad282), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_refresh_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_refresh_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_refresh_btn, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_refresh_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_refresh_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_refresh_btn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_refresh_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_refresh_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_refresh_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_bg_opa(ui->setting_refresh_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui->setting_refresh_btn, lv_color_hex(0x8aabc6), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(ui->setting_refresh_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_border_width(ui->setting_refresh_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_radius(ui->setting_refresh_btn, 5, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->setting_refresh_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->setting_refresh_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->setting_refresh_btn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->setting_refresh_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes setting_auto_connect_sw
    ui->setting_auto_connect_sw = lv_switch_create(ui->setting);
    lv_obj_set_pos(ui->setting_auto_connect_sw, 250, 80);
    lv_obj_set_size(ui->setting_auto_connect_sw, 65, 20);

    //Write style for setting_auto_connect_sw, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_auto_connect_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_auto_connect_sw, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_auto_connect_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_auto_connect_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_auto_connect_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_auto_connect_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_auto_connect_sw, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->setting_auto_connect_sw, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->setting_auto_connect_sw, lv_color_hex(0xffc700), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->setting_auto_connect_sw, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->setting_auto_connect_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for setting_auto_connect_sw, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_auto_connect_sw, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_auto_connect_sw, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_auto_connect_sw, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_auto_connect_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_auto_connect_sw, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes setting_auto_connect_label
    ui->setting_auto_connect_label = lv_label_create(ui->setting);
    lv_label_set_text(ui->setting_auto_connect_label, "自动连接");
    lv_label_set_long_mode(ui->setting_auto_connect_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->setting_auto_connect_label, 250, 60);
    lv_obj_set_size(ui->setting_auto_connect_label, 60, 13);

    //Write style for setting_auto_connect_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_auto_connect_label, lv_color_hex(0xff6500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_auto_connect_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_auto_connect_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_auto_connect_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_auto_connect_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


    //The custom code of setting.
    char *ssid = wifi_connect_ssid();
    if(wifi_connect_state())
    {
        lv_label_set_text_fmt(ui->setting_wifi_ssid_label, "已连接 %s", ssid);
    }
    else
    {
        lv_label_set_text(ui->setting_wifi_ssid_label, "未连接");
    }

    if (wifi_is_auto_connect_enabled()) 
    {
        lv_obj_add_state(ui->setting_auto_connect_sw, LV_STATE_CHECKED);
    } 
    else 
    {
        lv_obj_clear_state(ui->setting_auto_connect_sw, LV_STATE_CHECKED);
    }

    //Update current screen layout.
    lv_obj_update_layout(ui->setting);

    //Init events for screen.
    events_init_setting(ui);
}
