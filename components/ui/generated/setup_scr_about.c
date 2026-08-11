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



void setup_scr_about(lv_ui *ui)
{
    //Write codes about
    ui->about = lv_obj_create(NULL);
    lv_obj_set_size(ui->about, 320, 240);
    lv_obj_set_scrollbar_mode(ui->about, LV_SCROLLBAR_MODE_OFF);

    //Write style for about, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->about, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->about, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->about, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_back_imgbtn
    ui->about_back_imgbtn = lv_imgbtn_create(ui->about);
    lv_imgbtn_set_src(ui->about_back_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->about_back_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->about_back_imgbtn_label = lv_label_create(ui->about_back_imgbtn);
    lv_label_set_text(ui->about_back_imgbtn_label, "");
    lv_label_set_long_mode(ui->about_back_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->about_back_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->about_back_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->about_back_imgbtn, 5, 5);
    lv_obj_set_size(ui->about_back_imgbtn, 28, 28);

    //Write style for about_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->about_back_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_back_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_back_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->about_back_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for about_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->about_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->about_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->about_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->about_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->about_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->about_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for about_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->about_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->about_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->about_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->about_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->about_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->about_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for about_back_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->about_back_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->about_back_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes about_github_img
    ui->about_github_img = lv_img_create(ui->about);
    lv_obj_add_flag(ui->about_github_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->about_github_img, &_github_alpha_32x32);
    lv_img_set_pivot(ui->about_github_img, 50,50);
    lv_img_set_angle(ui->about_github_img, 0);
    lv_obj_set_pos(ui->about_github_img, 40, 6);
    lv_obj_set_size(ui->about_github_img, 32, 32);

    //Write style for about_github_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->about_github_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->about_github_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_github_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->about_github_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_github_label
    ui->about_github_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_github_label, "https://github.com/Chendeq/esp32s3_terminal");
    lv_label_set_long_mode(ui->about_github_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_github_label, 75, 15);
    lv_obj_set_size(ui->about_github_label, 240, 25);

    //Write style for about_github_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_github_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->about_github_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->about_github_label, lv_color_hex(0xede2e2), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->about_github_label, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_github_label, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_github_label, lv_color_hex(0x0055ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_github_label, &lv_font_Fangzhengcuheisongsimple_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_github_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_github_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_github_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_espressif_img
    ui->about_espressif_img = lv_img_create(ui->about);
    lv_obj_add_flag(ui->about_espressif_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->about_espressif_img, &_lex_alpha_32x32);
    lv_img_set_pivot(ui->about_espressif_img, 50,50);
    lv_img_set_angle(ui->about_espressif_img, 0);
    lv_obj_set_pos(ui->about_espressif_img, 145, 45);
    lv_obj_set_size(ui->about_espressif_img, 32, 32);

    //Write style for about_espressif_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->about_espressif_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->about_espressif_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_espressif_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->about_espressif_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_kfzc_label
    ui->about_kfzc_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_kfzc_label, "开发支持:");
    lv_label_set_long_mode(ui->about_kfzc_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_kfzc_label, 10, 55);
    lv_obj_set_size(ui->about_kfzc_label, 70, 15);

    //Write style for about_kfzc_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_kfzc_label, lv_color_hex(0xff8100), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_kfzc_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_kfzc_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_kfzc_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_kfzc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_vscode_img
    ui->about_vscode_img = lv_img_create(ui->about);
    lv_obj_add_flag(ui->about_vscode_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->about_vscode_img, &_Vscode_alpha_32x32);
    lv_img_set_pivot(ui->about_vscode_img, 50,50);
    lv_img_set_angle(ui->about_vscode_img, 0);
    lv_obj_set_pos(ui->about_vscode_img, 80, 45);
    lv_obj_set_size(ui->about_vscode_img, 32, 32);

    //Write style for about_vscode_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->about_vscode_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->about_vscode_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_vscode_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->about_vscode_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_label_1
    ui->about_label_1 = lv_label_create(ui->about);
    lv_label_set_text(ui->about_label_1, "+");
    lv_label_set_long_mode(ui->about_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_label_1, 125, 53);
    lv_obj_set_size(ui->about_label_1, 12, 16);

    //Write style for about_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_label_1, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_label_2
    ui->about_label_2 = lv_label_create(ui->about);
    lv_label_set_text(ui->about_label_2, "+");
    lv_label_set_long_mode(ui->about_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_label_2, 187, 53);
    lv_obj_set_size(ui->about_label_2, 12, 16);

    //Write style for about_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_label_2, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_xunfei_img
    ui->about_xunfei_img = lv_img_create(ui->about);
    lv_obj_add_flag(ui->about_xunfei_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->about_xunfei_img, &_xunfei_alpha_32x32);
    lv_img_set_pivot(ui->about_xunfei_img, 50,50);
    lv_img_set_angle(ui->about_xunfei_img, 0);
    lv_obj_set_pos(ui->about_xunfei_img, 208, 45);
    lv_obj_set_size(ui->about_xunfei_img, 32, 32);

    //Write style for about_xunfei_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->about_xunfei_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->about_xunfei_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_xunfei_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->about_xunfei_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_label_3
    ui->about_label_3 = lv_label_create(ui->about);
    lv_label_set_text(ui->about_label_3, "+");
    lv_label_set_long_mode(ui->about_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_label_3, 253, 53);
    lv_obj_set_size(ui->about_label_3, 12, 16);

    //Write style for about_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_label_3, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_guider_img
    ui->about_guider_img = lv_img_create(ui->about);
    lv_obj_add_flag(ui->about_guider_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->about_guider_img, &_gui_guider_alpha_32x32);
    lv_img_set_pivot(ui->about_guider_img, 50,50);
    lv_img_set_angle(ui->about_guider_img, 0);
    lv_obj_set_pos(ui->about_guider_img, 279, 45);
    lv_obj_set_size(ui->about_guider_img, 32, 32);

    //Write style for about_guider_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->about_guider_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->about_guider_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_guider_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->about_guider_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_lib_label
    ui->about_lib_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_lib_label, "开发库:ESP-IDF5.4.3 + LVGL8.3 + FreeRTOS");
    lv_label_set_long_mode(ui->about_lib_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_lib_label, 5, 88);
    lv_obj_set_size(ui->about_lib_label, 310, 15);

    //Write style for about_lib_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_lib_label, lv_color_hex(0x0039ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_lib_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_lib_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_lib_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_lib_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_platform_label
    ui->about_platform_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_platform_label, "硬件平台:自制ESP32S3开发板 + 320 X 240 LCD");
    lv_label_set_long_mode(ui->about_platform_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_platform_label, 10, 145);
    lv_obj_set_size(ui->about_platform_label, 310, 13);

    //Write style for about_platform_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_platform_label, lv_color_hex(0xff005e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_platform_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_platform_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_platform_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_platform_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_casnhu_label
    ui->about_casnhu_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_casnhu_label, "参数：");
    lv_label_set_long_mode(ui->about_casnhu_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_casnhu_label, 15, 165);
    lv_obj_set_size(ui->about_casnhu_label, 50, 15);

    //Write style for about_casnhu_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_casnhu_label, lv_color_hex(0xac0000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_casnhu_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_casnhu_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_casnhu_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_casnhu_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_flash_label
    ui->about_flash_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_flash_label, "FLASH:16MB");
    lv_label_set_long_mode(ui->about_flash_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_flash_label, 65, 165);
    lv_obj_set_size(ui->about_flash_label, 100, 15);

    //Write style for about_flash_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_flash_label, lv_color_hex(0xff6c00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_flash_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_flash_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_flash_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_flash_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_psram_label
    ui->about_psram_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_psram_label, "PSRAM:8MB");
    lv_label_set_long_mode(ui->about_psram_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_psram_label, 65, 185);
    lv_obj_set_size(ui->about_psram_label, 100, 15);

    //Write style for about_psram_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_psram_label, lv_color_hex(0xff6c00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_psram_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_psram_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_psram_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_psram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_isram_label
    ui->about_isram_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_isram_label, "ISRAM:512KB");
    lv_label_set_long_mode(ui->about_isram_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_isram_label, 65, 205);
    lv_obj_set_size(ui->about_isram_label, 100, 15);

    //Write style for about_isram_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_isram_label, lv_color_hex(0xff6c00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_isram_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_isram_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_isram_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_isram_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_lcd_label
    ui->about_lcd_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_lcd_label, "屏幕尺寸：2.8寸");
    lv_label_set_long_mode(ui->about_lcd_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_lcd_label, 185, 165);
    lv_obj_set_size(ui->about_lcd_label, 130, 15);

    //Write style for about_lcd_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_lcd_label, lv_color_hex(0xff6c00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_lcd_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_lcd_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_lcd_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_lcd_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_display_label
    ui->about_display_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_display_label, "DISPLAY:ST7789");
    lv_label_set_long_mode(ui->about_display_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_display_label, 185, 185);
    lv_obj_set_size(ui->about_display_label, 130, 15);

    //Write style for about_display_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_display_label, lv_color_hex(0xff6c00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_display_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_display_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_display_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_display_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_touch_label
    ui->about_touch_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_touch_label, "TOUCH:FT65536U");
    lv_label_set_long_mode(ui->about_touch_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_touch_label, 185, 205);
    lv_obj_set_size(ui->about_touch_label, 130, 15);

    //Write style for about_touch_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_touch_label, lv_color_hex(0xff6c00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_touch_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_touch_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_touch_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_touch_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_chip_label
    ui->about_chip_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_chip_label, "ESP32-S3-WROOM-1-N16R8");
    lv_label_set_long_mode(ui->about_chip_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->about_chip_label, 95, 115);
    lv_obj_set_size(ui->about_chip_label, 210, 20);

    //Write style for about_chip_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_chip_label, lv_color_hex(0xd700ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_chip_label, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_chip_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_chip_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_chip_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_chip_img
    ui->about_chip_img = lv_img_create(ui->about);
    lv_obj_add_flag(ui->about_chip_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->about_chip_img, &_chip_alpha_32x32);
    lv_img_set_pivot(ui->about_chip_img, 50,50);
    lv_img_set_angle(ui->about_chip_img, 0);
    lv_obj_set_pos(ui->about_chip_img, 44, 107);
    lv_obj_set_size(ui->about_chip_img, 32, 32);

    //Write style for about_chip_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->about_chip_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->about_chip_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_chip_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->about_chip_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes about_reference_label
    ui->about_reference_label = lv_label_create(ui->about);
    lv_label_set_text(ui->about_reference_label, "项目参考B站会打狙的船长、-犹豫忧郁鱿鱼-两位up主，非常感谢两位up能够开源代码！");
    lv_label_set_long_mode(ui->about_reference_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_pos(ui->about_reference_label, 0, 222);
    lv_obj_set_size(ui->about_reference_label, 320, 15);

    //Write style for about_reference_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->about_reference_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->about_reference_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->about_reference_label, lv_color_hex(0x063eff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->about_reference_label, LV_BORDER_SIDE_TOP, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->about_reference_label, lv_color_hex(0xff00f7), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->about_reference_label, &lv_font_Fangzhengcuheisongsimple_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->about_reference_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->about_reference_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->about_reference_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of about.


    //Update current screen layout.
    lv_obj_update_layout(ui->about);

    //Init events for screen.
    events_init_about(ui);
}
