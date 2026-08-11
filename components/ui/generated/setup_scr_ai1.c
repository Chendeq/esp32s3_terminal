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



void setup_scr_ai1(lv_ui *ui)
{
    //Write codes ai1
    ui->ai1 = lv_obj_create(NULL);
    lv_obj_set_size(ui->ai1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->ai1, LV_SCROLLBAR_MODE_OFF);

    //Write style for ai1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ai1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ai1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ai1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ai1_back_imgbtn
    ui->ai1_back_imgbtn = lv_imgbtn_create(ui->ai1);
    lv_imgbtn_set_src(ui->ai1_back_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->ai1_back_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->ai1_back_imgbtn_label = lv_label_create(ui->ai1_back_imgbtn);
    lv_label_set_text(ui->ai1_back_imgbtn_label, "");
    lv_label_set_long_mode(ui->ai1_back_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->ai1_back_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->ai1_back_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->ai1_back_imgbtn, 5, 5);
    lv_obj_set_size(ui->ai1_back_imgbtn, 28, 28);

    //Write style for ai1_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->ai1_back_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ai1_back_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ai1_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ai1_back_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->ai1_back_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ai1_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ai1_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->ai1_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->ai1_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->ai1_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->ai1_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->ai1_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for ai1_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->ai1_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->ai1_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->ai1_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->ai1_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->ai1_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for ai1_back_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_back_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->ai1_back_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes ai1_aitx_ta
    ui->ai1_aitx_ta = lv_textarea_create(ui->ai1);
    lv_textarea_set_text(ui->ai1_aitx_ta, "");
    lv_textarea_set_placeholder_text(ui->ai1_aitx_ta, "TX...");
    lv_textarea_set_password_bullet(ui->ai1_aitx_ta, "*");
    lv_textarea_set_password_mode(ui->ai1_aitx_ta, false);
    lv_textarea_set_one_line(ui->ai1_aitx_ta, false);
    lv_textarea_set_accepted_chars(ui->ai1_aitx_ta, "");
    lv_textarea_set_max_length(ui->ai1_aitx_ta, 1000);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->ai1_aitx_ta, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->ai1_aitx_ta, 50, 10);
    lv_obj_set_size(ui->ai1_aitx_ta, 225, 90);

    //Write style for ai1_aitx_ta, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->ai1_aitx_ta, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ai1_aitx_ta, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ai1_aitx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ai1_aitx_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ai1_aitx_ta, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ai1_aitx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ai1_aitx_ta, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ai1_aitx_ta, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ai1_aitx_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->ai1_aitx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->ai1_aitx_ta, lv_color_hex(0x00acf1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->ai1_aitx_ta, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ai1_aitx_ta, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ai1_aitx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ai1_aitx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ai1_aitx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_aitx_ta, 10, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ai1_aitx_ta, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ai1_aitx_ta, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ai1_aitx_ta, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ai1_aitx_ta, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_aitx_ta, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes ai1_airx_ta
    ui->ai1_airx_ta = lv_textarea_create(ui->ai1);
    lv_textarea_set_text(ui->ai1_airx_ta, "");
    lv_textarea_set_placeholder_text(ui->ai1_airx_ta, "RX...");
    lv_textarea_set_password_bullet(ui->ai1_airx_ta, "*");
    lv_textarea_set_password_mode(ui->ai1_airx_ta, false);
    lv_textarea_set_one_line(ui->ai1_airx_ta, false);
    lv_textarea_set_accepted_chars(ui->ai1_airx_ta, "");
    lv_textarea_set_max_length(ui->ai1_airx_ta, 3000);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->ai1_airx_ta, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->ai1_airx_ta, 80, 110);
    lv_obj_set_size(ui->ai1_airx_ta, 225, 120);

    //Write style for ai1_airx_ta, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->ai1_airx_ta, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ai1_airx_ta, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ai1_airx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ai1_airx_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ai1_airx_ta, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ai1_airx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ai1_airx_ta, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ai1_airx_ta, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ai1_airx_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->ai1_airx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->ai1_airx_ta, lv_color_hex(0x00ff93), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->ai1_airx_ta, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ai1_airx_ta, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ai1_airx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ai1_airx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ai1_airx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_airx_ta, 10, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ai1_airx_ta, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ai1_airx_ta, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ai1_airx_ta, lv_color_hex(0xffffff), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ai1_airx_ta, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_airx_ta, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    lv_obj_add_flag(ui->ai1_airx_ta, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui->ai1_airx_ta, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->ai1_airx_ta, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_set_scroll_dir(ui->ai1_airx_ta, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui->ai1_airx_ta, LV_SCROLLBAR_MODE_AUTO);
    lv_textarea_set_cursor_click_pos(ui->ai1_airx_ta, false);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_remove_event_cb(ui->ai1_airx_ta, ta_event_cb);
#endif

    //Write codes ai1_send_imgbtn
    ui->ai1_send_imgbtn = lv_imgbtn_create(ui->ai1);
    lv_imgbtn_set_src(ui->ai1_send_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_send_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->ai1_send_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_send_p_alpha_32x32, NULL);
    ui->ai1_send_imgbtn_label = lv_label_create(ui->ai1_send_imgbtn);
    lv_label_set_text(ui->ai1_send_imgbtn_label, "");
    lv_label_set_long_mode(ui->ai1_send_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->ai1_send_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->ai1_send_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->ai1_send_imgbtn, 10, 45);
    lv_obj_set_size(ui->ai1_send_imgbtn, 32, 32);

    //Write style for ai1_send_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->ai1_send_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ai1_send_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ai1_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ai1_send_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->ai1_send_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ai1_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ai1_send_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->ai1_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->ai1_send_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->ai1_send_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->ai1_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->ai1_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for ai1_send_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->ai1_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->ai1_send_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->ai1_send_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->ai1_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->ai1_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for ai1_send_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_send_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->ai1_send_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes ai1_clear_imgbtn
    ui->ai1_clear_imgbtn = lv_imgbtn_create(ui->ai1);
    lv_imgbtn_set_src(ui->ai1_clear_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_clear_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->ai1_clear_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_clear_p_alpha_32x32, NULL);
    ui->ai1_clear_imgbtn_label = lv_label_create(ui->ai1_clear_imgbtn);
    lv_label_set_text(ui->ai1_clear_imgbtn_label, "");
    lv_label_set_long_mode(ui->ai1_clear_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->ai1_clear_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->ai1_clear_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->ai1_clear_imgbtn, 35, 175);
    lv_obj_set_size(ui->ai1_clear_imgbtn, 32, 32);

    //Write style for ai1_clear_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->ai1_clear_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ai1_clear_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ai1_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ai1_clear_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->ai1_clear_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ai1_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ai1_clear_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->ai1_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->ai1_clear_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->ai1_clear_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->ai1_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->ai1_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for ai1_clear_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->ai1_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->ai1_clear_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->ai1_clear_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->ai1_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->ai1_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for ai1_clear_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->ai1_clear_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->ai1_clear_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes ai1_aibq_img
    ui->ai1_aibq_img = lv_img_create(ui->ai1);
    lv_obj_add_flag(ui->ai1_aibq_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->ai1_aibq_img, &_aibq_alpha_32x32);
    lv_img_set_pivot(ui->ai1_aibq_img, 50,50);
    lv_img_set_angle(ui->ai1_aibq_img, 0);
    lv_obj_set_pos(ui->ai1_aibq_img, 26, 124);
    lv_obj_set_size(ui->ai1_aibq_img, 32, 32);

    //Write style for ai1_aibq_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->ai1_aibq_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->ai1_aibq_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_aibq_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->ai1_aibq_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ai1_user_img
    ui->ai1_user_img = lv_img_create(ui->ai1);
    lv_obj_add_flag(ui->ai1_user_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->ai1_user_img, &_user_alpha_32x32);
    lv_img_set_pivot(ui->ai1_user_img, 50,50);
    lv_img_set_angle(ui->ai1_user_img, 0);
    lv_obj_set_pos(ui->ai1_user_img, 281, 35);
    lv_obj_set_size(ui->ai1_user_img, 32, 32);

    //Write style for ai1_user_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->ai1_user_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->ai1_user_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ai1_user_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->ai1_user_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of ai1.


    //Update current screen layout.
    lv_obj_update_layout(ui->ai1);

    //Init events for screen.
    events_init_ai1(ui);
}
