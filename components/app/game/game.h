#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "gui_guider.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GAME_TYPE_2048 = 0,
    GAME_TYPE_YANG,
    GAME_TYPE_PVZ,
    GAME_TYPE_XIAOXIAOLE,
} game_type_t;

esp_err_t game_score_init(void);
uint32_t game_get_best_score(void);
esp_err_t game_try_update_best_score(uint32_t current_score,
                                     uint32_t *out_best_score,
                                     bool *out_updated);

void game_select(game_type_t type);
void game_start_selected(lv_ui *ui);
void game_stop_current(void);

#ifdef __cplusplus
}
#endif

#endif
