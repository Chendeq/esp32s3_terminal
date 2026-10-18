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



void setup_scr_record(lv_ui *ui)
{
    //Write codes record
    ui->record = lv_obj_create(NULL);
    lv_obj_set_size(ui->record, 320, 240);
    lv_obj_set_scrollbar_mode(ui->record, LV_SCROLLBAR_MODE_OFF);

    //Write style for record, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->record, 215, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->record, lv_color_hex(0xfcfafa), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->record, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes record_record_imgbtn
    ui->record_record_imgbtn = lv_imgbtn_create(ui->record);
    lv_imgbtn_set_src(ui->record_record_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->record_record_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->record_record_imgbtn_label = lv_label_create(ui->record_record_imgbtn);
    lv_label_set_text(ui->record_record_imgbtn_label, "");
    lv_label_set_long_mode(ui->record_record_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->record_record_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->record_record_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->record_record_imgbtn, 5, 5);
    lv_obj_set_size(ui->record_record_imgbtn, 28, 28);

    //Write style for record_record_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->record_record_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->record_record_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->record_record_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->record_record_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->record_record_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->record_record_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->record_record_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for record_record_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->record_record_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->record_record_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->record_record_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->record_record_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->record_record_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->record_record_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for record_record_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->record_record_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->record_record_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->record_record_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->record_record_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->record_record_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->record_record_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for record_record_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->record_record_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->record_record_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes record_rec_start_btn
    ui->record_rec_start_btn = lv_btn_create(ui->record);
    ui->record_rec_start_btn_label = lv_label_create(ui->record_rec_start_btn);
    lv_label_set_text(ui->record_rec_start_btn_label, "Start REC");
    lv_label_set_long_mode(ui->record_rec_start_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->record_rec_start_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->record_rec_start_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->record_rec_start_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->record_rec_start_btn, 15, 45);
    lv_obj_set_size(ui->record_rec_start_btn, 85, 30);

    //Write style for record_rec_start_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->record_rec_start_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->record_rec_start_btn, lv_color_hex(0x009bff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->record_rec_start_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->record_rec_start_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->record_rec_start_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->record_rec_start_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->record_rec_start_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->record_rec_start_btn, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->record_rec_start_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->record_rec_start_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes record_rec_stop_btn
    ui->record_rec_stop_btn = lv_btn_create(ui->record);
    ui->record_rec_stop_btn_label = lv_label_create(ui->record_rec_stop_btn);
    lv_label_set_text(ui->record_rec_stop_btn_label, "Stop REC");
    lv_label_set_long_mode(ui->record_rec_stop_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->record_rec_stop_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->record_rec_stop_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->record_rec_stop_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->record_rec_stop_btn, 115, 45);
    lv_obj_set_size(ui->record_rec_stop_btn, 85, 30);

    //Write style for record_rec_stop_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->record_rec_stop_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->record_rec_stop_btn, lv_color_hex(0x009bff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->record_rec_stop_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->record_rec_stop_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->record_rec_stop_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->record_rec_stop_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->record_rec_stop_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->record_rec_stop_btn, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->record_rec_stop_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->record_rec_stop_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_state(ui->record_rec_stop_btn, LV_STATE_DISABLED);

    //Write codes record_duration_label
    ui->record_duration_label = lv_label_create(ui->record);
    lv_label_set_text(ui->record_duration_label, "");
    lv_label_set_long_mode(ui->record_duration_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->record_duration_label, 210, 45);
    lv_obj_set_size(ui->record_duration_label, 100, 30);

    //Write style for record_duration_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->record_duration_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->record_duration_label, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->record_duration_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->record_duration_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->record_duration_label, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->record_duration_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes record_record_list
    ui->record_record_list = lv_list_create(ui->record);
    ui->record_record_list_item0 =lv_list_add_text(ui->record_record_list, "save_1");
    lv_obj_set_pos(ui->record_record_list, 10, 90);
    lv_obj_set_size(ui->record_record_list, 300, 115);
    lv_obj_set_scrollbar_mode(ui->record_record_list, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_record_record_list_main_main_default
    static lv_style_t style_record_record_list_main_main_default;
    ui_init_style(&style_record_record_list_main_main_default);

    lv_style_set_pad_top(&style_record_record_list_main_main_default, 5);
    lv_style_set_pad_left(&style_record_record_list_main_main_default, 5);
    lv_style_set_pad_right(&style_record_record_list_main_main_default, 5);
    lv_style_set_pad_bottom(&style_record_record_list_main_main_default, 5);
    lv_style_set_bg_opa(&style_record_record_list_main_main_default, 255);
    lv_style_set_bg_color(&style_record_record_list_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_record_record_list_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_record_record_list_main_main_default, 1);
    lv_style_set_border_opa(&style_record_record_list_main_main_default, 255);
    lv_style_set_border_color(&style_record_record_list_main_main_default, lv_color_hex(0x6f6f6f));
    lv_style_set_border_side(&style_record_record_list_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_record_record_list_main_main_default, 3);
    lv_style_set_shadow_width(&style_record_record_list_main_main_default, 0);
    lv_obj_add_style(ui->record_record_list, &style_record_record_list_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_record_record_list_main_scrollbar_default
    static lv_style_t style_record_record_list_main_scrollbar_default;
    ui_init_style(&style_record_record_list_main_scrollbar_default);

    lv_style_set_radius(&style_record_record_list_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_record_record_list_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_record_record_list_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_record_record_list_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->record_record_list, &style_record_record_list_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_record_record_list_extra_btns_main_default
    static lv_style_t style_record_record_list_extra_btns_main_default;
    ui_init_style(&style_record_record_list_extra_btns_main_default);

    lv_style_set_pad_top(&style_record_record_list_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_record_record_list_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_record_record_list_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_record_record_list_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_record_record_list_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_record_record_list_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_record_record_list_extra_btns_main_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_record_record_list_extra_btns_main_default, 255);
    lv_style_set_radius(&style_record_record_list_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_record_record_list_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_record_record_list_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_record_record_list_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_record_record_list_extra_texts_main_default
    static lv_style_t style_record_record_list_extra_texts_main_default;
    ui_init_style(&style_record_record_list_extra_texts_main_default);

    lv_style_set_pad_top(&style_record_record_list_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_record_record_list_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_record_record_list_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_record_record_list_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_record_record_list_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_record_record_list_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_record_record_list_extra_texts_main_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_record_record_list_extra_texts_main_default, 255);
    lv_style_set_radius(&style_record_record_list_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_record_record_list_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_record_record_list_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_record_record_list_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_record_record_list_extra_texts_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->record_record_list_item0, &style_record_record_list_extra_texts_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes record_rec_state_label
    ui->record_rec_state_label = lv_label_create(ui->record);
    lv_label_set_text(ui->record_rec_state_label, "Ready");
    lv_label_set_long_mode(ui->record_rec_state_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->record_rec_state_label, 10, 213);
    lv_obj_set_size(ui->record_rec_state_label, 90, 21);

    //Write style for record_rec_state_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->record_rec_state_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->record_rec_state_label, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->record_rec_state_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->record_rec_state_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->record_rec_state_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes record_msg_label
    ui->record_msg_label = lv_label_create(ui->record);
    lv_label_set_text(ui->record_msg_label, "WAV PCM16 | Gain x32");
    lv_label_set_long_mode(ui->record_msg_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->record_msg_label, 112, 213);
    lv_obj_set_size(ui->record_msg_label, 200, 21);

    //Write style for record_msg_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->record_msg_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->record_msg_label, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->record_msg_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->record_msg_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->record_msg_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of record.


    //Update current screen layout.
    lv_obj_update_layout(ui->record);

    //Init events for screen.
    events_init_record(ui);
}
