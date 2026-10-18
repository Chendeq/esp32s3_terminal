#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MUSIC_MAX_FILES         128
#define MUSIC_PATH_MAX_LEN      272
#define MUSIC_NAME_MAX_LEN      128

/* Compatibility aliases for existing UI code. */
#define MP3_PATH_MAX_LEN        MUSIC_PATH_MAX_LEN
#define MP3_NAME_MAX_LEN        MUSIC_NAME_MAX_LEN

typedef enum {
    MUSIC_CMD_PLAY_PAUSE,
    MUSIC_CMD_PREV,
    MUSIC_CMD_NEXT,
    MUSIC_CMD_PLAY_INDEX,
    MUSIC_CMD_VOLUME,
    MUSIC_CMD_STOP,
    MUSIC_CMD_SCAN,
    MUSIC_CMD_PLAY_FILE,
} music_cmd_code_t;

typedef struct {
    music_cmd_code_t cmd;
    union {
        uint16_t index;
        uint8_t volume;
        char path[MUSIC_PATH_MAX_LEN];
    } data;
} music_cmd_msg_t;

typedef enum {
    MUSIC_EVT_STARTED,
    MUSIC_EVT_PAUSED,
    MUSIC_EVT_RESUMED,
    MUSIC_EVT_STOPPED,
    MUSIC_EVT_SONG_END,
    MUSIC_EVT_SCAN_DONE,
    MUSIC_EVT_VOLUME_CHANGED,
    MUSIC_EVT_PROGRESS,
} music_event_id_t;

extern QueueHandle_t g_music_cmd_queue;

typedef void (*pf_music_event_cb)(music_event_id_t event_id);

esp_err_t music_init(void);
void music_scan_files(void);

uint16_t music_get_count(void);
const char *music_get_name(uint16_t index);
int16_t music_get_current_index(void);
const char *music_get_current_name(void);

void music_set_volume(uint8_t percent);
uint8_t music_get_volume(void);

bool music_is_paused(void);
bool music_is_playing(void);
uint32_t music_get_total_time_ms(void);
/* Reads metadata with a separate decoder, leaving current playback untouched. */
bool music_get_file_duration_ms(const char *path, uint32_t *duration_ms);
uint32_t music_get_elapsed_time_ms(void);
uint8_t music_get_progress_percent(void);
uint16_t music_get_lyric_count(void);
int16_t music_get_current_lyric_index(void);
const char *music_get_lyric_line(uint16_t index);

BaseType_t music_send_cmd(const music_cmd_msg_t *msg);
void music_register_handler(pf_music_event_cb handler);
void music_play(void);

#ifdef __cplusplus
}
#endif

#endif
