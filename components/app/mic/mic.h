#ifndef APP_MIC_H
#define APP_MIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "esp_err.h"
#include "driver/i2s_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIC_I2S_PORT            I2S_NUM_1
#define MIC_WS_GPIO             13
#define MIC_SCK_GPIO            11
#define MIC_SD_GPIO             12
#define MIC_SAMPLE_RATE         16000U
#define MIC_FRAME_SAMPLES       256U
#define MIC_RECORD_GAIN         32U
#define MIC_WAV_DIRECTORY       "/S/WAV"
#define MIC_RECORD_PATH_MAX     96U

typedef enum {
    MIC_RECORD_IDLE = 0,
    MIC_RECORD_STARTING,
    MIC_RECORDING,
    MIC_RECORD_STOPPING,
    MIC_RECORD_SAVED,
    MIC_RECORD_ERROR,
} mic_record_state_t;

typedef struct {
    mic_record_state_t state;
    esp_err_t error;
    uint32_t data_bytes;
    uint32_t saved_count;
    char path[MIC_RECORD_PATH_MAX];
} mic_record_status_t;

typedef struct {
    time_t modified;
    char name[256];
} mic_wav_file_t;

esp_err_t mic_init(void);
void mic_deinit(void);
/* Task entry points; creation and stack allocation belong to app_task.c. */
void mic_writer_task(void *arg);
void mic_capture_task(void *arg);
esp_err_t mic_record_start(void);
esp_err_t mic_record_stop(void);
bool mic_get_record_status(mic_record_status_t *status);
uint32_t mic_get_record_elapsed_ms(void);
/* Snapshot of /S/WAV, newest first. Release it with mic_free_wav_files(). */
esp_err_t mic_get_wav_files(mic_wav_file_t **files, size_t *count);
void mic_free_wav_files(mic_wav_file_t *files);

#ifdef __cplusplus
}
#endif

#endif
