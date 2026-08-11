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



void setup_scr_gamechose(lv_ui *ui)
{
    //Write codes gamechose
    ui->gamechose = lv_obj_create(NULL);
    lv_obj_set_size(ui->gamechose, 320, 240);
    lv_obj_set_scrollbar_mode(ui->gamechose, LV_SCROLLBAR_MODE_OFF);

    //Write style for gamechose, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->gamechose, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->gamechose, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->gamechose, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes gamechose_back_imgbtn
    ui->gamechose_back_imgbtn = lv_imgbtn_create(ui->gamechose);
    lv_imgbtn_set_src(ui->gamechose_back_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->gamechose_back_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->gamechose_back_imgbtn_label = lv_label_create(ui->gamechose_back_imgbtn);
    lv_label_set_text(ui->gamechose_back_imgbtn_label, "");
    lv_label_set_long_mode(ui->gamechose_back_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->gamechose_back_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->gamechose_back_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->gamechose_back_imgbtn, 5, 5);
    lv_obj_set_size(ui->gamechose_back_imgbtn, 28, 28);

    //Write style for gamechose_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->gamechose_back_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->gamechose_back_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->gamechose_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->gamechose_back_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->gamechose_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->gamechose_back_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->gamechose_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for gamechose_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->gamechose_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->gamechose_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->gamechose_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->gamechose_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->gamechose_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->gamechose_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for gamechose_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->gamechose_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->gamechose_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->gamechose_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->gamechose_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->gamechose_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->gamechose_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for gamechose_back_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->gamechose_back_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->gamechose_back_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes gamechose_btn_2048
    ui->gamechose_btn_2048 = lv_btn_create(ui->gamechose);
    ui->gamechose_btn_2048_label = lv_label_create(ui->gamechose_btn_2048);
    lv_label_set_text(ui->gamechose_btn_2048_label, "2048");
    lv_label_set_long_mode(ui->gamechose_btn_2048_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->gamechose_btn_2048_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->gamechose_btn_2048, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->gamechose_btn_2048_label, LV_PCT(100));
    lv_obj_set_pos(ui->gamechose_btn_2048, 30, 60);
    lv_obj_set_size(ui->gamechose_btn_2048, 100, 40);

    //Write style for gamechose_btn_2048, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->gamechose_btn_2048, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->gamechose_btn_2048, lv_color_hex(0xe8ab0a), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->gamechose_btn_2048, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->gamechose_btn_2048, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->gamechose_btn_2048, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->gamechose_btn_2048, lv_color_hex(0xa3b0ba), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->gamechose_btn_2048, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->gamechose_btn_2048, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->gamechose_btn_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->gamechose_btn_2048, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->gamechose_btn_2048, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->gamechose_btn_2048, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->gamechose_btn_2048, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes gamechose_yang_btn
    ui->gamechose_yang_btn = lv_btn_create(ui->gamechose);
    ui->gamechose_yang_btn_label = lv_label_create(ui->gamechose_yang_btn);
    lv_label_set_text(ui->gamechose_yang_btn_label, "羊了个羊");
    lv_label_set_long_mode(ui->gamechose_yang_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->gamechose_yang_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->gamechose_yang_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->gamechose_yang_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->gamechose_yang_btn, 190, 60);
    lv_obj_set_size(ui->gamechose_yang_btn, 100, 40);

    //Write style for gamechose_yang_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->gamechose_yang_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->gamechose_yang_btn, lv_color_hex(0x28e80a), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->gamechose_yang_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->gamechose_yang_btn, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->gamechose_yang_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->gamechose_yang_btn, lv_color_hex(0xa9b9c5), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->gamechose_yang_btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->gamechose_yang_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->gamechose_yang_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->gamechose_yang_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->gamechose_yang_btn, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->gamechose_yang_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->gamechose_yang_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes gamechose_zhi_btn
    ui->gamechose_zhi_btn = lv_btn_create(ui->gamechose);
    ui->gamechose_zhi_btn_label = lv_label_create(ui->gamechose_zhi_btn);
    lv_label_set_text(ui->gamechose_zhi_btn_label, "植物大战僵尸");
    lv_label_set_long_mode(ui->gamechose_zhi_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->gamechose_zhi_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->gamechose_zhi_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->gamechose_zhi_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->gamechose_zhi_btn, 190, 150);
    lv_obj_set_size(ui->gamechose_zhi_btn, 100, 40);

    //Write style for gamechose_zhi_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->gamechose_zhi_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->gamechose_zhi_btn, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->gamechose_zhi_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->gamechose_zhi_btn, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->gamechose_zhi_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->gamechose_zhi_btn, lv_color_hex(0xb4bcc3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->gamechose_zhi_btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->gamechose_zhi_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->gamechose_zhi_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->gamechose_zhi_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->gamechose_zhi_btn, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->gamechose_zhi_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->gamechose_zhi_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes gamechose_xiao_btn
    ui->gamechose_xiao_btn = lv_btn_create(ui->gamechose);
    ui->gamechose_xiao_btn_label = lv_label_create(ui->gamechose_xiao_btn);
    lv_label_set_text(ui->gamechose_xiao_btn_label, "消消乐");
    lv_label_set_long_mode(ui->gamechose_xiao_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->gamechose_xiao_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->gamechose_xiao_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->gamechose_xiao_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->gamechose_xiao_btn, 30, 150);
    lv_obj_set_size(ui->gamechose_xiao_btn, 100, 40);

    //Write style for gamechose_xiao_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->gamechose_xiao_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->gamechose_xiao_btn, lv_color_hex(0xaa18e0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->gamechose_xiao_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->gamechose_xiao_btn, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->gamechose_xiao_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->gamechose_xiao_btn, lv_color_hex(0xa9b9c5), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->gamechose_xiao_btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->gamechose_xiao_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->gamechose_xiao_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->gamechose_xiao_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->gamechose_xiao_btn, &lv_font_Fangzhengcuheisongsimple_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->gamechose_xiao_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->gamechose_xiao_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of gamechose.


    //Update current screen layout.
    lv_obj_update_layout(ui->gamechose);

    //Init events for screen.
    events_init_gamechose(ui);
}
