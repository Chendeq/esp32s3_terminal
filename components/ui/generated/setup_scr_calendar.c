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
#include "app_ui.h"


lv_calendar_date_t calendar_calendar_1_today;
lv_calendar_date_t calendar_calendar_1_highlihted_days[1];
void setup_scr_calendar(lv_ui *ui)
{
    //Write codes calendar
    ui->calendar = lv_obj_create(NULL);
    lv_obj_set_size(ui->calendar, 320, 240);
    lv_obj_set_scrollbar_mode(ui->calendar, LV_SCROLLBAR_MODE_OFF);

    //Write style for calendar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->calendar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->calendar, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->calendar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes calendar_calendar_1
    ui->calendar_calendar_1 = lv_calendar_create(ui->calendar);
    calendar_calendar_1_today.year = 2026;
    calendar_calendar_1_today.month = 5;
    calendar_calendar_1_today.day = 2;
    lv_calendar_set_today_date(ui->calendar_calendar_1, calendar_calendar_1_today.year, calendar_calendar_1_today.month, calendar_calendar_1_today.day);
    lv_calendar_set_showed_date(ui->calendar_calendar_1, calendar_calendar_1_today.year, calendar_calendar_1_today.month);
    calendar_calendar_1_highlihted_days[0].year = 2026;
    calendar_calendar_1_highlihted_days[0].month = 5;
    calendar_calendar_1_highlihted_days[0].day = 3;
    lv_calendar_set_highlighted_dates(ui->calendar_calendar_1, calendar_calendar_1_highlihted_days, 1);
    lv_obj_t *calendar_calendar_1_header = lv_calendar_header_arrow_create(ui->calendar_calendar_1);
    lv_calendar_t *calendar_calendar_1 = (lv_calendar_t *)ui->calendar_calendar_1;
    lv_obj_add_event_cb(calendar_calendar_1->btnm, calendar_calendar_1_draw_part_begin_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
    lv_obj_add_event_cb(ui->calendar_calendar_1, calendar_calendar_1_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->calendar_calendar_1, 10, 35);
    lv_obj_set_size(ui->calendar_calendar_1, 300, 200);

    //Write style state: LV_STATE_DEFAULT for &style_calendar_calendar_1_main_main_default
    static lv_style_t style_calendar_calendar_1_main_main_default;
    ui_init_style(&style_calendar_calendar_1_main_main_default);

    lv_style_set_border_width(&style_calendar_calendar_1_main_main_default, 1);
    lv_style_set_border_opa(&style_calendar_calendar_1_main_main_default, 255);
    lv_style_set_border_color(&style_calendar_calendar_1_main_main_default, lv_color_hex(0xb1b1b1));
    lv_style_set_border_side(&style_calendar_calendar_1_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_bg_opa(&style_calendar_calendar_1_main_main_default, 255);
    lv_style_set_bg_color(&style_calendar_calendar_1_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_calendar_calendar_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_calendar_calendar_1_main_main_default, 0);
    lv_style_set_radius(&style_calendar_calendar_1_main_main_default, 0);
    lv_obj_add_style(ui->calendar_calendar_1, &style_calendar_calendar_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_calendar_calendar_1_extra_header_main_default
    static lv_style_t style_calendar_calendar_1_extra_header_main_default;
    ui_init_style(&style_calendar_calendar_1_extra_header_main_default);

    lv_style_set_text_color(&style_calendar_calendar_1_extra_header_main_default, lv_color_hex(0x000000));
    lv_style_set_text_font(&style_calendar_calendar_1_extra_header_main_default, &lv_font_Fangzhengcuheisongsimple_16);
    lv_style_set_text_opa(&style_calendar_calendar_1_extra_header_main_default, 255);
    lv_style_set_bg_opa(&style_calendar_calendar_1_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_calendar_calendar_1_extra_header_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_calendar_calendar_1_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(calendar_calendar_1_header, &style_calendar_calendar_1_extra_header_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_calendar_calendar_1_main_items_default
    static lv_style_t style_calendar_calendar_1_main_items_default;
    ui_init_style(&style_calendar_calendar_1_main_items_default);

    lv_style_set_bg_opa(&style_calendar_calendar_1_main_items_default, 0);
    lv_style_set_border_width(&style_calendar_calendar_1_main_items_default, 1);
    lv_style_set_border_opa(&style_calendar_calendar_1_main_items_default, 255);
    lv_style_set_border_color(&style_calendar_calendar_1_main_items_default, lv_color_hex(0xc0c0c0));
    lv_style_set_border_side(&style_calendar_calendar_1_main_items_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_calendar_calendar_1_main_items_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_calendar_calendar_1_main_items_default, &lv_font_Fangzhengcuheisongsimple_14);
    lv_style_set_text_opa(&style_calendar_calendar_1_main_items_default, 255);
    lv_obj_add_style(lv_calendar_get_btnmatrix(ui->calendar_calendar_1), &style_calendar_calendar_1_main_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);

    //Write codes calendar_back_imgbtn
    ui->calendar_back_imgbtn = lv_imgbtn_create(ui->calendar);
    lv_imgbtn_set_src(ui->calendar_back_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_back_release_alpha_28x28, NULL);
    lv_imgbtn_set_src(ui->calendar_back_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_back_press_alpha_28x28, NULL);
    ui->calendar_back_imgbtn_label = lv_label_create(ui->calendar_back_imgbtn);
    lv_label_set_text(ui->calendar_back_imgbtn_label, "");
    lv_label_set_long_mode(ui->calendar_back_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->calendar_back_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->calendar_back_imgbtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->calendar_back_imgbtn, 5, 5);
    lv_obj_set_size(ui->calendar_back_imgbtn, 28, 28);

    //Write style for calendar_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->calendar_back_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->calendar_back_imgbtn, &lv_font_Fangzhengcuheisongsimple_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->calendar_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->calendar_back_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->calendar_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->calendar_back_imgbtn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->calendar_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for calendar_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->calendar_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->calendar_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->calendar_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->calendar_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->calendar_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->calendar_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for calendar_back_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->calendar_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->calendar_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->calendar_back_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->calendar_back_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->calendar_back_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->calendar_back_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for calendar_back_imgbtn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->calendar_back_imgbtn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->calendar_back_imgbtn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //The custom code of calendar.

    // 更新日历显示
    update_calendar_from_time();

    //Update current screen layout.
    lv_obj_update_layout(ui->calendar);

    //Init events for screen.
    events_init_calendar(ui);
}
