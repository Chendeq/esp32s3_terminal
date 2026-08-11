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
#include "serial.h"



void setup_scr_serial(lv_ui *ui)
{
    //Write codes serial
    ui->serial = lv_obj_create(NULL);
    lv_obj_set_size(ui->serial, 320, 240);
    lv_obj_set_scrollbar_mode(ui->serial, LV_SCROLLBAR_MODE_OFF);

    //Write style for serial, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->serial, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->serial, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->serial, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes serial_back_imgbtn
    ui->serial_back_imgbtn = lv_imgbtn_create(ui->serial);
    lv_imgbtn_set_src(ui->serial_back_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->serial_back_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->serial_back_imgbtn_label = lv_label_create(ui->serial_back_imgbtn);
    lv_label_set_text(ui->serial_back_imgbtn_label, "");
    lv_label_set_long_mode(ui->serial_back_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->serial_back_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->serial_back_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->serial_back_imgbtn, 5, 5);
    lv_obj_set_size(ui->serial_back_imgbtn, 28, 28);

    //Write style for serial_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->serial_back_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->serial_back_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->serial_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->serial_back_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->serial_back_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->serial_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for serial_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->serial_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->serial_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->serial_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->serial_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->serial_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->serial_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for serial_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->serial_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->serial_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->serial_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->serial_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->serial_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->serial_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for serial_back_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->serial_back_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->serial_back_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes serial_tx_ta
    ui->serial_tx_ta = lv_textarea_create(ui->serial);
    lv_textarea_set_text(ui->serial_tx_ta, "");
    lv_textarea_set_placeholder_text(ui->serial_tx_ta, "TX...");
    lv_textarea_set_password_bullet(ui->serial_tx_ta, "*");
    lv_textarea_set_password_mode(ui->serial_tx_ta, false);
    lv_textarea_set_one_line(ui->serial_tx_ta, false);
    lv_textarea_set_accepted_chars(ui->serial_tx_ta, "");
    lv_textarea_set_max_length(ui->serial_tx_ta, 1000);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->serial_tx_ta, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->serial_tx_ta, 20, 35);
    lv_obj_set_size(ui->serial_tx_ta, 250, 80);

    //Write style for serial_tx_ta, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->serial_tx_ta, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->serial_tx_ta, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->serial_tx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->serial_tx_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->serial_tx_ta, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->serial_tx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->serial_tx_ta, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->serial_tx_ta, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->serial_tx_ta, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->serial_tx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->serial_tx_ta, lv_color_hex(0xffb900), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->serial_tx_ta, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->serial_tx_ta, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->serial_tx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->serial_tx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->serial_tx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_tx_ta, 10, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for serial_tx_ta, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->serial_tx_ta, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->serial_tx_ta, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->serial_tx_ta, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_tx_ta, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes serial_rx_ta
    ui->serial_rx_ta = lv_textarea_create(ui->serial);
    lv_textarea_set_text(ui->serial_rx_ta, "");
    lv_textarea_set_placeholder_text(ui->serial_rx_ta, "RX...");
    lv_textarea_set_password_bullet(ui->serial_rx_ta, "*");
    lv_textarea_set_password_mode(ui->serial_rx_ta, false);
    lv_textarea_set_one_line(ui->serial_rx_ta, false);
    lv_textarea_set_accepted_chars(ui->serial_rx_ta, "");
    lv_textarea_set_max_length(ui->serial_rx_ta, 3000);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->serial_rx_ta, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->serial_rx_ta, 20, 133);
    lv_obj_set_size(ui->serial_rx_ta, 250, 100);
    lv_obj_add_flag(ui->serial_rx_ta, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for serial_rx_ta, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->serial_rx_ta, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->serial_rx_ta, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->serial_rx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->serial_rx_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->serial_rx_ta, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->serial_rx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->serial_rx_ta, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->serial_rx_ta, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->serial_rx_ta, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->serial_rx_ta, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->serial_rx_ta, lv_color_hex(0x98ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->serial_rx_ta, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->serial_rx_ta, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->serial_rx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->serial_rx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->serial_rx_ta, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_rx_ta, 10, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for serial_rx_ta, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->serial_rx_ta, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->serial_rx_ta, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->serial_rx_ta, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_rx_ta, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes serial_send_imgbtn
    ui->serial_send_imgbtn = lv_imgbtn_create(ui->serial);
    lv_imgbtn_set_src(ui->serial_send_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_send_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->serial_send_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_send_p_alpha_32x32, NULL);
    ui->serial_send_imgbtn_label = lv_label_create(ui->serial_send_imgbtn);
    lv_label_set_text(ui->serial_send_imgbtn_label, "");
    lv_label_set_long_mode(ui->serial_send_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->serial_send_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->serial_send_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->serial_send_imgbtn, 280, 50);
    lv_obj_set_size(ui->serial_send_imgbtn, 32, 32);

    //Write style for serial_send_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->serial_send_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->serial_send_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->serial_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->serial_send_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->serial_send_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->serial_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for serial_send_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->serial_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->serial_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->serial_send_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->serial_send_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->serial_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->serial_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for serial_send_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->serial_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->serial_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->serial_send_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->serial_send_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->serial_send_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->serial_send_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for serial_send_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->serial_send_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->serial_send_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes serial_clear_imgbtn
    ui->serial_clear_imgbtn = lv_imgbtn_create(ui->serial);
    lv_imgbtn_set_src(ui->serial_clear_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_clear_alpha_32x32, NULL);
    lv_imgbtn_set_src(ui->serial_clear_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_clear_p_alpha_32x32, NULL);
    ui->serial_clear_imgbtn_label = lv_label_create(ui->serial_clear_imgbtn);
    lv_label_set_text(ui->serial_clear_imgbtn_label, "");
    lv_label_set_long_mode(ui->serial_clear_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->serial_clear_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->serial_clear_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->serial_clear_imgbtn, 280, 170);
    lv_obj_set_size(ui->serial_clear_imgbtn, 32, 32);

    //Write style for serial_clear_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->serial_clear_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->serial_clear_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->serial_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->serial_clear_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->serial_clear_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->serial_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for serial_clear_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->serial_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->serial_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->serial_clear_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->serial_clear_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->serial_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->serial_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for serial_clear_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->serial_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->serial_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->serial_clear_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->serial_clear_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->serial_clear_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->serial_clear_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for serial_clear_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->serial_clear_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->serial_clear_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes serial_baud_ddlist
    ui->serial_baud_ddlist = lv_dropdown_create(ui->serial);
    lv_dropdown_set_options(ui->serial_baud_ddlist, "115200\n9600\n19200\n38400\n57600");
    lv_dropdown_set_symbol(ui->serial_baud_ddlist, NULL);
    lv_obj_set_pos(ui->serial_baud_ddlist, 245, 5);
    lv_obj_set_size(ui->serial_baud_ddlist, 70, 20);

    //Write style for serial_baud_ddlist, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->serial_baud_ddlist, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->serial_baud_ddlist, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->serial_baud_ddlist, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->serial_baud_ddlist, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->serial_baud_ddlist, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->serial_baud_ddlist, lv_color_hex(0xe5cbcb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->serial_baud_ddlist, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->serial_baud_ddlist, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->serial_baud_ddlist, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->serial_baud_ddlist, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->serial_baud_ddlist, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->serial_baud_ddlist, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->serial_baud_ddlist, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->serial_baud_ddlist, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->serial_baud_ddlist, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_serial_baud_ddlist_extra_list_selected_checked
    static lv_style_t style_serial_baud_ddlist_extra_list_selected_checked;
    ui_init_style(&style_serial_baud_ddlist_extra_list_selected_checked);

    lv_style_set_border_width(&style_serial_baud_ddlist_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_serial_baud_ddlist_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_serial_baud_ddlist_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_serial_baud_ddlist_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_serial_baud_ddlist_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_serial_baud_ddlist_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_serial_baud_ddlist_extra_list_selected_checked, lv_color_hex(0x889ff9));
    lv_style_set_bg_grad_dir(&style_serial_baud_ddlist_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->serial_baud_ddlist), &style_serial_baud_ddlist_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_serial_baud_ddlist_extra_list_main_default
    static lv_style_t style_serial_baud_ddlist_extra_list_main_default;
    ui_init_style(&style_serial_baud_ddlist_extra_list_main_default);

    lv_style_set_max_height(&style_serial_baud_ddlist_extra_list_main_default, 90);
    lv_style_set_text_color(&style_serial_baud_ddlist_extra_list_main_default, lv_color_hex(0x000000));
    lv_style_set_text_font(&style_serial_baud_ddlist_extra_list_main_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_serial_baud_ddlist_extra_list_main_default, 255);
    lv_style_set_border_width(&style_serial_baud_ddlist_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_serial_baud_ddlist_extra_list_main_default, 255);
    lv_style_set_border_color(&style_serial_baud_ddlist_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_serial_baud_ddlist_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_serial_baud_ddlist_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_serial_baud_ddlist_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_serial_baud_ddlist_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_serial_baud_ddlist_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->serial_baud_ddlist), &style_serial_baud_ddlist_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_serial_baud_ddlist_extra_list_scrollbar_default
    static lv_style_t style_serial_baud_ddlist_extra_list_scrollbar_default;
    ui_init_style(&style_serial_baud_ddlist_extra_list_scrollbar_default);

    lv_style_set_radius(&style_serial_baud_ddlist_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_serial_baud_ddlist_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_serial_baud_ddlist_extra_list_scrollbar_default, lv_color_hex(0x889ff9));
    lv_style_set_bg_grad_dir(&style_serial_baud_ddlist_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->serial_baud_ddlist), &style_serial_baud_ddlist_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //The custom code of serial.

    if (ui->serial_rx_ta) {
    lv_obj_add_flag(ui->serial_rx_ta, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui->serial_rx_ta, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->serial_rx_ta, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_set_scroll_dir(ui->serial_rx_ta, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui->serial_rx_ta, LV_SCROLLBAR_MODE_AUTO);
    lv_textarea_set_cursor_click_pos(ui->serial_rx_ta, false);
#if LV_USE_KEYBOARD
    lv_obj_remove_event_cb(ui->serial_rx_ta, ta_event_cb);
#endif
    }

    serial_port_init();

    //Update current screen layout.
    lv_obj_update_layout(ui->serial);

    //Init events for screen.
    events_init_serial(ui);
}
