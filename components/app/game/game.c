#include "game.h"

#include "lv_100ask_2048.h"
#include "pvz.h"
#include "xiaoxiaole.h"
#include "yang.h"

#include "nvs.h"
#include "nvs_flash.h"
#include <stdio.h>

#define GAME_NVS_NAMESPACE  "game2048"
#define GAME_SCORE_SLOT_MAX 256

static game_type_t s_selected_type = GAME_TYPE_2048;
static game_type_t s_running_type = GAME_TYPE_2048;
static lv_obj_t *s_game_root;
static lv_obj_t *s_2048_obj;
static lv_obj_t *s_2048_score_label;
static lv_obj_t *s_2048_best_label;
static lv_obj_t *s_2048_status_label;

static bool g_score_inited;
static uint32_t g_best_score;
static uint16_t g_next_slot;

static void game_make_slot_key(uint16_t slot, char key[16])
{
    snprintf(key, 16, "s%04u", (unsigned)slot);
}

static esp_err_t game_scan_slots(nvs_handle_t nvs)
{
    uint32_t score = 0;

    g_best_score = 0;
    g_next_slot = 0;

    for (uint16_t i = 0; i < GAME_SCORE_SLOT_MAX; i++) {
        char key[16];
        game_make_slot_key(i, key);

        esp_err_t err = nvs_get_u32(nvs, key, &score);
        if (err == ESP_OK) {
            if (score > g_best_score) {
                g_best_score = score;
            }
            g_next_slot = (uint16_t)(i + 1);
            continue;
        }

        if (err == ESP_ERR_NVS_NOT_FOUND) {
            g_next_slot = i;
            return ESP_OK;
        }

        return err;
    }

    return ESP_OK;
}

esp_err_t game_score_init(void)
{
    if (g_score_inited) {
        return ESP_OK;
    }

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        return err;
    }

    nvs_handle_t nvs;
    err = nvs_open(GAME_NVS_NAMESPACE, NVS_READWRITE, &nvs);
    if (err != ESP_OK) {
        return err;
    }

    err = game_scan_slots(nvs);
    nvs_close(nvs);
    if (err != ESP_OK) {
        return err;
    }

    g_score_inited = true;
    return ESP_OK;
}

uint32_t game_get_best_score(void)
{
    return g_best_score;
}

esp_err_t game_try_update_best_score(uint32_t current_score,
                                     uint32_t *out_best_score,
                                     bool *out_updated)
{
    esp_err_t err = game_score_init();
    if (err != ESP_OK) {
        return err;
    }

    if (out_updated != NULL) {
        *out_updated = false;
    }

    if (current_score <= g_best_score) {
        if (out_best_score != NULL) {
            *out_best_score = g_best_score;
        }
        return ESP_OK;
    }

    nvs_handle_t nvs;
    err = nvs_open(GAME_NVS_NAMESPACE, NVS_READWRITE, &nvs);
    if (err != ESP_OK) {
        return err;
    }

    if (g_next_slot >= GAME_SCORE_SLOT_MAX) {
        err = nvs_erase_all(nvs);
        if (err == ESP_OK) {
            err = nvs_commit(nvs);
        }
        if (err != ESP_OK) {
            nvs_close(nvs);
            return err;
        }
        g_next_slot = 0;
    }

    char key[16];
    game_make_slot_key(g_next_slot, key);
    err = nvs_set_u32(nvs, key, current_score);
    if (err == ESP_OK) {
        err = nvs_commit(nvs);
    }
    nvs_close(nvs);

    if (err != ESP_OK) {
        return err;
    }

    g_best_score = current_score;
    g_next_slot++;

    if (out_best_score != NULL) {
        *out_best_score = g_best_score;
    }
    if (out_updated != NULL) {
        *out_updated = true;
    }

    return ESP_OK;
}

static void game_2048_update_labels(void)
{
#if LV_USE_100ASK_2048 != 0
    uint32_t score = lv_100ask_2048_get_score(s_2048_obj);
    uint32_t best_score = game_get_best_score();

    if (game_try_update_best_score(score, &best_score, NULL) != ESP_OK) {
        best_score = game_get_best_score();
    }

    lv_label_set_text_fmt(s_2048_score_label, "Score: %lu", (unsigned long)score);
    lv_label_set_text_fmt(s_2048_best_label, "Best: %lu", (unsigned long)best_score);
    lv_label_set_text(s_2048_status_label, lv_100ask_2048_get_status(s_2048_obj) ? "Game Over" : "");
#endif
}

static void game_2048_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        game_2048_update_labels();
    }
}

static void game_2048_restart_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
#if LV_USE_100ASK_2048 != 0
        lv_100ask_2048_set_new_game(s_2048_obj);
        game_2048_update_labels();
#endif
    }
}

static void game_start_2048(lv_obj_t *parent)
{
#if LV_USE_100ASK_2048 != 0
    lv_obj_t *header = lv_obj_create(parent);
    lv_obj_remove_style_all(header);
    lv_obj_set_size(header, 280, 36);
    lv_obj_align(header, LV_ALIGN_TOP_RIGHT, -4, 2);

    s_2048_score_label = lv_label_create(header);
    lv_label_set_text(s_2048_score_label, "Score: 0");
    lv_obj_set_pos(s_2048_score_label, 0, 0);

    s_2048_best_label = lv_label_create(header);
    lv_label_set_text(s_2048_best_label, "Best: 0");
    lv_obj_set_pos(s_2048_best_label, 0, 18);

    lv_obj_t *restart_btn = lv_btn_create(header);
    lv_obj_set_size(restart_btn, 74, 28);
    lv_obj_align(restart_btn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(restart_btn, game_2048_restart_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *restart_label = lv_label_create(restart_btn);
    lv_label_set_text(restart_label, "Restart");
    lv_obj_center(restart_label);

    s_2048_obj = lv_100ask_2048_create(parent);
    lv_obj_set_size(s_2048_obj, 190, 190);
    lv_obj_align(s_2048_obj, LV_ALIGN_BOTTOM_MID, 0, -4);
    lv_obj_add_event_cb(s_2048_obj, game_2048_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_flag(s_2048_obj, LV_OBJ_FLAG_CLICKABLE);

    s_2048_status_label = lv_label_create(parent);
    lv_obj_align(s_2048_status_label, LV_ALIGN_TOP_MID, 0, 39);
    lv_obj_set_style_text_color(s_2048_status_label, lv_color_hex(0xff3b30), LV_PART_MAIN);

    game_score_init();
    game_2048_update_labels();
#else
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, "2048 disabled");
    lv_obj_center(label);
#endif
}

void game_select(game_type_t type)
{
    s_selected_type = type;
}

void game_stop_current(void)
{
    switch (s_running_type) {
    case GAME_TYPE_PVZ:
        pvz_stop();
        break;
    case GAME_TYPE_XIAOXIAOLE:
        xiaoxiaole_stop();
        break;
    case GAME_TYPE_YANG:
        yang_stop();
        break;
    case GAME_TYPE_2048:
    default:
        break;
    }

    if (s_game_root != NULL && lv_obj_is_valid(s_game_root)) {
        lv_obj_del(s_game_root);
    }

    s_game_root = NULL;
    s_2048_obj = NULL;
    s_2048_score_label = NULL;
    s_2048_best_label = NULL;
    s_2048_status_label = NULL;
    s_running_type = GAME_TYPE_2048;
}

void game_start_selected(lv_ui *ui)
{
    if (ui == NULL || ui->gameplay == NULL) {
        return;
    }

    game_stop_current();

    s_running_type = s_selected_type;
    s_game_root = lv_obj_create(ui->gameplay);
    lv_obj_remove_style_all(s_game_root);
    lv_obj_set_size(s_game_root, 320, 240);
    lv_obj_set_pos(s_game_root, 0, 0);
    lv_obj_clear_flag(s_game_root, LV_OBJ_FLAG_SCROLLABLE);

    switch (s_selected_type) {
    case GAME_TYPE_YANG:
        yang_game(s_game_root);
        break;
    case GAME_TYPE_PVZ:
        pvz_start(s_game_root);
        break;
    case GAME_TYPE_XIAOXIAOLE:
        xiaoxiaole(s_game_root);
        break;
    case GAME_TYPE_2048:
    default:
        game_start_2048(s_game_root);
        break;
    }

    if (ui->gameplay_back_imgbtn != NULL) {
        lv_obj_move_foreground(ui->gameplay_back_imgbtn);
    }
}
