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
#include "esp_heap_caps.h"
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

static void *s_music_lyric_canvas_buf = NULL;



void setup_scr_music(lv_ui *ui)
{
    //Write codes music
    ui->music = lv_obj_create(NULL);
    lv_obj_set_size(ui->music, 320, 240);
    lv_obj_set_scrollbar_mode(ui->music, LV_SCROLLBAR_MODE_OFF);

    //Write style for music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music, lv_color_hex(0xf1eded), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_back_imgbtn
    ui->music_back_imgbtn = lv_imgbtn_create(ui->music);
    lv_imgbtn_set_src(ui->music_back_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->music_back_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->music_back_imgbtn_label = lv_label_create(ui->music_back_imgbtn);
    lv_label_set_text(ui->music_back_imgbtn_label, "");
    lv_label_set_long_mode(ui->music_back_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_back_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_back_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_back_imgbtn, 5, 5);
    lv_obj_set_size(ui->music_back_imgbtn, 28, 28);

    //Write style for music_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_back_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_back_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_back_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_back_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_back_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_back_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_back_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_volume_slider
    ui->music_volume_slider = lv_slider_create(ui->music);
    lv_slider_set_range(ui->music_volume_slider, 0, 100);
    lv_slider_set_mode(ui->music_volume_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->music_volume_slider, 50, LV_ANIM_OFF);
    lv_obj_set_pos(ui->music_volume_slider, 30, 65);
    lv_obj_set_size(ui->music_volume_slider, 10, 110);

    //Write style for music_volume_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_volume_slider, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_volume_slider, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_volume_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_volume_slider, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->music_volume_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_volume_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_volume_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_volume_slider, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_volume_slider, lv_color_hex(0x16ee80), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_volume_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_volume_slider, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for music_volume_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_volume_slider, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_volume_slider, lv_color_hex(0x288cf1), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_volume_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_volume_slider, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes music_volume_label
    ui->music_volume_label = lv_label_create(ui->music);
    lv_label_set_text(ui->music_volume_label, "音量");
    lv_label_set_long_mode(ui->music_volume_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->music_volume_label, 17, 180);
    lv_obj_set_size(ui->music_volume_label, 35, 15);

    //Write style for music_volume_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_volume_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_volume_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_volume_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_volume_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_volume_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_music_now_label
    ui->music_music_now_label = lv_label_create(ui->music);
    lv_label_set_text(ui->music_music_now_label, "明日坐标");
    lv_label_set_long_mode(ui->music_music_now_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_pos(ui->music_music_now_label, 80, 15);
    lv_obj_set_size(ui->music_music_now_label, 200, 20);

    //Write style for music_music_now_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_music_now_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->music_music_now_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->music_music_now_label, lv_color_hex(0xd4e1ed), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->music_music_now_label, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_music_now_label, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_music_now_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_music_now_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_music_now_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_music_now_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_music_now_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_music_list
    ui->music_music_list = lv_list_create(ui->music);
    lv_obj_set_pos(ui->music_music_list, 70, 40);
    lv_obj_set_size(ui->music_music_list, 230, 130);
    lv_obj_set_scrollbar_mode(ui->music_music_list, LV_SCROLLBAR_MODE_ON);

    //Write style state: LV_STATE_DEFAULT for &style_music_music_list_main_main_default
    static lv_style_t style_music_music_list_main_main_default;
    ui_init_style(&style_music_music_list_main_main_default);

    lv_style_set_pad_top(&style_music_music_list_main_main_default, 5);
    lv_style_set_pad_left(&style_music_music_list_main_main_default, 5);
    lv_style_set_pad_right(&style_music_music_list_main_main_default, 5);
    lv_style_set_pad_bottom(&style_music_music_list_main_main_default, 5);
    lv_style_set_bg_opa(&style_music_music_list_main_main_default, 255);
    lv_style_set_bg_color(&style_music_music_list_main_main_default, lv_color_hex(0xfaecec));
    lv_style_set_bg_grad_dir(&style_music_music_list_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_music_music_list_main_main_default, 2);
    lv_style_set_border_opa(&style_music_music_list_main_main_default, 255);
    lv_style_set_border_color(&style_music_music_list_main_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_music_music_list_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_music_music_list_main_main_default, 5);
    lv_style_set_shadow_width(&style_music_music_list_main_main_default, 0);
    lv_obj_add_style(ui->music_music_list, &style_music_music_list_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_music_music_list_main_scrollbar_default
    static lv_style_t style_music_music_list_main_scrollbar_default;
    ui_init_style(&style_music_music_list_main_scrollbar_default);

    lv_style_set_radius(&style_music_music_list_main_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_music_music_list_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_music_music_list_main_scrollbar_default, lv_color_hex(0xfaecec));
    lv_style_set_bg_grad_dir(&style_music_music_list_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->music_music_list, &style_music_music_list_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_music_music_list_extra_btns_main_default
    static lv_style_t style_music_music_list_extra_btns_main_default;
    ui_init_style(&style_music_music_list_extra_btns_main_default);

    lv_style_set_pad_top(&style_music_music_list_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_music_music_list_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_music_music_list_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_music_music_list_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_music_music_list_extra_btns_main_default, 1);
    lv_style_set_border_opa(&style_music_music_list_extra_btns_main_default, 255);
    lv_style_set_border_color(&style_music_music_list_extra_btns_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_music_music_list_extra_btns_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_music_music_list_extra_btns_main_default, lv_color_hex(0x000000));
    lv_style_set_text_font(&style_music_music_list_extra_btns_main_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_music_music_list_extra_btns_main_default, 255);
    lv_style_set_radius(&style_music_music_list_extra_btns_main_default, 5);
    lv_style_set_bg_opa(&style_music_music_list_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_music_music_list_extra_btns_main_default, lv_color_hex(0xfcf8f8));
    lv_style_set_bg_grad_dir(&style_music_music_list_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_music_music_list_extra_texts_main_default
    static lv_style_t style_music_music_list_extra_texts_main_default;
    ui_init_style(&style_music_music_list_extra_texts_main_default);

    lv_style_set_pad_top(&style_music_music_list_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_music_music_list_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_music_music_list_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_music_music_list_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_music_music_list_extra_texts_main_default, 1);
    lv_style_set_border_opa(&style_music_music_list_extra_texts_main_default, 255);
    lv_style_set_border_color(&style_music_music_list_extra_texts_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_music_music_list_extra_texts_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_music_music_list_extra_texts_main_default, lv_color_hex(0x000000));
    lv_style_set_text_font(&style_music_music_list_extra_texts_main_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_music_music_list_extra_texts_main_default, 255);
    lv_style_set_radius(&style_music_music_list_extra_texts_main_default, 5);
    lv_style_set_transform_width(&style_music_music_list_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_music_music_list_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_music_music_list_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_music_music_list_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes music_play_control__imgbtn
    ui->music_play_control__imgbtn = lv_imgbtn_create(ui->music);
    lv_obj_add_flag(ui->music_play_control__imgbtn, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->music_play_control__imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_play_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->music_play_control__imgbtn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_pause_alpha_32x32, NULL);
    ui->music_play_control__imgbtn_label = lv_label_create(ui->music_play_control__imgbtn);
    lv_label_set_text(ui->music_play_control__imgbtn_label, "");
    lv_label_set_long_mode(ui->music_play_control__imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_play_control__imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_play_control__imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_play_control__imgbtn, 160, 180);
    lv_obj_set_size(ui->music_play_control__imgbtn, 32, 32);

    //Write style for music_play_control__imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_play_control__imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_play_control__imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_play_control__imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_play_control__imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_play_control__imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_play_control__imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_play_control__imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_play_control__imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_play_control__imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_play_control__imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_play_control__imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_play_control__imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_play_control__imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_play_control__imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_play_control__imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_play_control__imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_play_control__imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_play_control__imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_play_control__imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_play_control__imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_play_control__imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_play_control__imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_play_control__imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_play_control__imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_previous_imgbtn
    ui->music_previous_imgbtn = lv_imgbtn_create(ui->music);
    lv_imgbtn_set_src(ui->music_previous_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_previous_song_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->music_previous_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_previous_press_alpha_32x32, NULL);
    ui->music_previous_imgbtn_label = lv_label_create(ui->music_previous_imgbtn);
    lv_label_set_text(ui->music_previous_imgbtn_label, "");
    lv_label_set_long_mode(ui->music_previous_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_previous_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_previous_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_previous_imgbtn, 100, 180);
    lv_obj_set_size(ui->music_previous_imgbtn, 32, 32);

    //Write style for music_previous_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_previous_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_previous_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_previous_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_previous_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_previous_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_previous_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_previous_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_previous_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_previous_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_previous_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_previous_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_previous_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_previous_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_previous_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_previous_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_previous_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_previous_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_previous_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_previous_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_previous_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_previous_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_previous_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_previous_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_previous_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_next_imgbtn
    ui->music_next_imgbtn = lv_imgbtn_create(ui->music);
    lv_imgbtn_set_src(ui->music_next_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_next_song_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->music_next_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_next_press_alpha_32x32, NULL);
    ui->music_next_imgbtn_label = lv_label_create(ui->music_next_imgbtn);
    lv_label_set_text(ui->music_next_imgbtn_label, "");
    lv_label_set_long_mode(ui->music_next_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_next_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_next_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_next_imgbtn, 220, 180);
    lv_obj_set_size(ui->music_next_imgbtn, 32, 32);

    //Write style for music_next_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_next_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_next_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_next_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_next_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_next_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_next_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_next_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_next_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_next_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_next_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_next_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_next_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_next_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_next_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_next_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_next_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_next_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_next_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_next_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_next_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_next_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_next_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_next_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_next_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_volume_number_label
    ui->music_volume_number_label = lv_label_create(ui->music);
    lv_label_set_text(ui->music_volume_number_label, "100");
    lv_label_set_long_mode(ui->music_volume_number_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->music_volume_number_label, 15, 40);
    lv_obj_set_size(ui->music_volume_number_label, 40, 18);

    //Write style for music_volume_number_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_volume_number_label, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_volume_number_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_volume_number_label, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_volume_number_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_volume_number_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_volume_number_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_lyric_imgbtn
    ui->music_lyric_imgbtn = lv_imgbtn_create(ui->music);
    lv_imgbtn_set_src(ui->music_lyric_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_lyric_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->music_lyric_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_lyric_alpha_32x32, NULL);
    ui->music_lyric_imgbtn_label = lv_label_create(ui->music_lyric_imgbtn);
    lv_label_set_text(ui->music_lyric_imgbtn_label, "");
    lv_label_set_long_mode(ui->music_lyric_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_lyric_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_lyric_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_lyric_imgbtn, 280, 180);
    lv_obj_set_size(ui->music_lyric_imgbtn, 32, 32);

    //Write style for music_lyric_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->music_lyric_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_lyric_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_lyric_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_lyric_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_lyric_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_lyric_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_lyric_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_lyric_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_text_color(ui->music_lyric_imgbtn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_lyric_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_lyric_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_lyric_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor_opa(ui->music_lyric_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_lyric_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_lyric_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_lyric_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_lyric_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_lyric_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_lyric_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_lyric_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_lyric_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_lyric_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_lyric_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_lyric_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_lyric_cont
    ui->music_lyric_cont = lv_obj_create(ui->music);
    lv_obj_set_pos(ui->music_lyric_cont, 0, 0);
    lv_obj_set_size(ui->music_lyric_cont, 320, 240);
    lv_obj_set_scrollbar_mode(ui->music_lyric_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->music_lyric_cont, LV_OBJ_FLAG_HIDDEN);

    //Write style for music_lyric_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_lyric_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_lyric_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_lyric_cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_lyric_cont, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_lyric_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_lyric_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_lyric_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_lyric_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_lyric_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_lyric_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_lyric_canvas
    ui->music_lyric_canvas = lv_canvas_create(ui->music_lyric_cont);
    if (s_music_lyric_canvas_buf == NULL) {
        s_music_lyric_canvas_buf = heap_caps_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(300, 170),
                                                    MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    if (s_music_lyric_canvas_buf != NULL) {
        lv_canvas_set_buffer(ui->music_lyric_canvas, s_music_lyric_canvas_buf, 300, 170,
                             LV_IMG_CF_TRUE_COLOR);
    }
    lv_canvas_fill_bg(ui->music_lyric_canvas, lv_color_hex(0xc3c1c1), 255);
    lv_obj_set_pos(ui->music_lyric_canvas, 10, 30);
    lv_obj_set_size(ui->music_lyric_canvas, 300, 170);
    lv_obj_set_scrollbar_mode(ui->music_lyric_canvas, LV_SCROLLBAR_MODE_OFF);

    //Write codes music_lyric_cancel_imgbtn
    ui->music_lyric_cancel_imgbtn = lv_imgbtn_create(ui->music_lyric_cont);
    lv_imgbtn_set_src(ui->music_lyric_cancel_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_cancel_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->music_lyric_cancel_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_cancel_press_alpha_20x20, NULL);
    ui->music_lyric_cancel_imgbtn_label = lv_label_create(ui->music_lyric_cancel_imgbtn);
    lv_label_set_text(ui->music_lyric_cancel_imgbtn_label, "");
    lv_label_set_long_mode(ui->music_lyric_cancel_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_lyric_cancel_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_lyric_cancel_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_lyric_cancel_imgbtn, 295, 4);
    lv_obj_set_size(ui->music_lyric_cancel_imgbtn, 16, 16);

    //Write style for music_lyric_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_lyric_cancel_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_lyric_cancel_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_lyric_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_lyric_cancel_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_lyric_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_lyric_cancel_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_lyric_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_lyric_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_lyric_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_lyric_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_lyric_cancel_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_lyric_cancel_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_lyric_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_lyric_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_lyric_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_lyric_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_lyric_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_lyric_cancel_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_lyric_cancel_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_lyric_cancel_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_lyric_cancel_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_lyric_cancel_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_lyric_cancel_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_lyric_cancel_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_totaltime_label
    ui->music_totaltime_label = lv_label_create(ui->music_lyric_cont);
    lv_label_set_text(ui->music_totaltime_label, "04:32");
    lv_label_set_long_mode(ui->music_totaltime_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->music_totaltime_label, 270, 215);
    lv_obj_set_size(ui->music_totaltime_label, 35, 10);

    //Write style for music_totaltime_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_totaltime_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_totaltime_label, &lv_font_Fangzhengcuheisongsimple_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_totaltime_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_totaltime_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_totaltime_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_time_play_label
    ui->music_time_play_label = lv_label_create(ui->music_lyric_cont);
    lv_label_set_text(ui->music_time_play_label, "00:00");
    lv_label_set_long_mode(ui->music_time_play_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->music_time_play_label, 15, 215);
    lv_obj_set_size(ui->music_time_play_label, 35, 10);

    //Write style for music_time_play_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_time_play_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_time_play_label, &lv_font_Fangzhengcuheisongsimple_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_time_play_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_time_play_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_time_play_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_play_progress_slider
    ui->music_play_progress_slider = lv_slider_create(ui->music_lyric_cont);
    lv_slider_set_range(ui->music_play_progress_slider, 0, 100);
    lv_slider_set_mode(ui->music_play_progress_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->music_play_progress_slider, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->music_play_progress_slider, 62, 220);
    lv_obj_set_size(ui->music_play_progress_slider, 196, 4);

    //Write style for music_play_progress_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_play_progress_slider, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_play_progress_slider, lv_color_hex(0x95abbd), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_play_progress_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_play_progress_slider, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->music_play_progress_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_play_progress_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_play_progress_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_play_progress_slider, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_play_progress_slider, lv_color_hex(0x82ff94), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_play_progress_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_play_progress_slider, 5, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for music_play_progress_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_play_progress_slider, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_play_progress_slider, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_play_progress_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_play_progress_slider, 8, LV_PART_KNOB|LV_STATE_DEFAULT);


    //The custom code of music.


    //Update current screen layout.
    lv_obj_update_layout(ui->music);

    //Init events for screen.
    events_init_music(ui);
}
