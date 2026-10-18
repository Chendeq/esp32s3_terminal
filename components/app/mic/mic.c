#include "mic.h"

#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bsp_sdcard.h"
#include "app_task.h"
#include "datetime.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "format_wav.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"

static const char *TAG = "mic";

#define MIC_QUEUE_BLOCKS 128U

typedef struct {
    int16_t samples[MIC_FRAME_SAMPLES];
} mic_block_t;

static i2s_chan_handle_t s_rx_handle;
static QueueHandle_t s_queue;
static SemaphoreHandle_t s_capture_done;
static SemaphoreHandle_t s_writer_done;
static TaskHandle_t s_capture_task;
static TaskHandle_t s_writer_task;
static portMUX_TYPE s_lock = portMUX_INITIALIZER_UNLOCKED;
static mic_record_status_t s_status;
static uint64_t s_record_start_us;
static uint64_t s_record_end_us;
/* True from the capture notification until I2S has stopped and the producer is idle. */
static bool s_capture_active;
static volatile bool s_running;
static uint32_t s_file_sequence;
static char s_latest_wav_path[MIC_RECORD_PATH_MAX];

static void status_set(mic_record_state_t state, esp_err_t error)
{
    portENTER_CRITICAL(&s_lock);
    if (s_status.state == MIC_RECORD_STARTING ||
        s_status.state == MIC_RECORDING ||
        s_status.state == MIC_RECORD_STOPPING) {
        if (state == MIC_RECORD_STOPPING && s_record_end_us == 0) {
            s_record_end_us = esp_timer_get_time();
        }
        s_status.state = state;
        if (error != ESP_OK) {
            s_status.error = error;
        }
    }
    portEXIT_CRITICAL(&s_lock);
}

static esp_err_t open_wav(FILE **file, char *path, size_t path_size, char *pending, size_t pending_size)
{
    esp_err_t err = sdcard_is_mounted() ? ESP_OK : sdcard_init();
    if (err != ESP_OK || !sdcard_is_mounted()) {
        return err != ESP_OK ? err : ESP_FAIL;
    }

    struct stat st;
    if (mkdir(MIC_WAV_DIRECTORY, 0775) != 0 && errno != EEXIST) {
        ESP_LOGE(TAG, "mkdir %s failed: %d", MIC_WAV_DIRECTORY, errno);
        return ESP_FAIL;
    }
    if (stat(MIC_WAV_DIRECTORY, &st) != 0 || !S_ISDIR(st.st_mode)) {
        return ESP_FAIL;
    }

    char stamp[64];
    if (datetime_is_time_synced()) {
        static const char *weekdays[] = {"星期日", "星期一", "星期二", "星期三",
                                         "星期四", "星期五", "星期六"};
        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        snprintf(stamp, sizeof(stamp), "%04d-%02d-%02d_%s_%02d-%02d-%02d",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, weekdays[tm.tm_wday],
                 tm.tm_hour, tm.tm_min, tm.tm_sec);
    } else {
        snprintf(stamp, sizeof(stamp), "%lld", (long long)(esp_timer_get_time() / 1000));
    }

    for (;;) {
        int length = snprintf(path, path_size, MIC_WAV_DIRECTORY "/record_%s_%lu.wav",
                              stamp, (unsigned long)++s_file_sequence);
        if (length < 0 || (size_t)length >= path_size) {
            return ESP_ERR_INVALID_SIZE;
        }
        length = snprintf(pending, pending_size, "%s.part", path);
        if (length < 0 || (size_t)length >= pending_size) {
            return ESP_ERR_INVALID_SIZE;
        }
        if (stat(path, &st) == 0) {
            continue;
        }
        if (errno != ENOENT) {
            return ESP_FAIL;
        }

        /* Publish .wav only after the header and PCM data have been closed successfully. */
        int fd = open(pending, O_CREAT | O_EXCL | O_WRONLY, 0666);
        if (fd < 0) {
            if (errno == EEXIST) {
                continue;
            }
            ESP_LOGE(TAG, "open %s failed: %d", path, errno);
            return ESP_FAIL;
        }

        *file = fdopen(fd, "wb");
        if (*file == NULL) {
            close(fd);
            unlink(pending);
            return ESP_FAIL;
        }

        wav_header_t header = WAV_HEADER_PCM_DEFAULT(0, 16, MIC_SAMPLE_RATE, 1);
        if (fwrite(&header, sizeof(header), 1, *file) != 1 || fflush(*file) != 0) {
            fclose(*file);
            *file = NULL;
            unlink(pending);
            return ESP_FAIL;
        }
        return ESP_OK;
    }
}

static esp_err_t close_wav(FILE *file, uint32_t data_bytes)
{
    wav_header_t header = WAV_HEADER_PCM_DEFAULT(data_bytes, 16, MIC_SAMPLE_RATE, 1);
    esp_err_t err = ESP_OK;

    clearerr(file);
    if (fseek(file, 0, SEEK_SET) != 0 ||
        fwrite(&header, sizeof(header), 1, file) != 1 || fflush(file) != 0) {
        err = ESP_FAIL;
    }
    if (fclose(file) != 0) {
        err = ESP_FAIL;
    }
    return err;
}

void mic_writer_task(void *arg)
{
    (void)arg;
    FILE *file = NULL;
    uint32_t data_bytes = 0;
    char path[MIC_RECORD_PATH_MAX] = {0};
    char pending[MIC_RECORD_PATH_MAX + 5] = {0};
    mic_block_t block;

    for (;;) {
        mic_record_status_t status;
        mic_get_record_status(&status);

        if (file == NULL && (status.state == MIC_RECORD_STARTING ||
                             status.state == MIC_RECORD_STOPPING)) {
            xQueueReset(s_queue);
            path[0] = '\0';
            data_bytes = 0;
            esp_err_t err = open_wav(&file, path, sizeof(path), pending, sizeof(pending));
            portENTER_CRITICAL(&s_lock);
            s_status.error = err;
            if (err == ESP_OK) {
                snprintf(s_status.path, sizeof(s_status.path), "%s", path);
                /* A stop during SD open must not be overwritten by RECORDING. */
                s_capture_active = s_running && s_status.state == MIC_RECORD_STARTING;
                s_status.state = s_capture_active ? MIC_RECORDING : MIC_RECORD_STOPPING;
            } else {
                s_status.state = MIC_RECORD_ERROR;
            }
            bool start_capture = s_capture_active;
            portEXIT_CRITICAL(&s_lock);
            if (start_capture) {
                xTaskNotifyGive(s_capture_task);
            } else if (err != ESP_OK) {
                ESP_LOGE(TAG, "record start failed: %s", esp_err_to_name(err));
            }
            continue;
        }

        if (file == NULL) {
            if (!s_running && status.state != MIC_RECORD_STARTING) {
                break;
            }
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            continue;
        }

        if (xQueueReceive(s_queue, &block, pdMS_TO_TICKS(20)) == pdTRUE) {
            size_t written = fwrite(block.samples, sizeof(int16_t), MIC_FRAME_SAMPLES, file);
            data_bytes += (uint32_t)(written * sizeof(int16_t));
            portENTER_CRITICAL(&s_lock);
            s_status.data_bytes = data_bytes;
            portEXIT_CRITICAL(&s_lock);
            if (written != MIC_FRAME_SAMPLES) {
                status_set(MIC_RECORD_STOPPING, ESP_FAIL);
                ESP_LOGE(TAG, "WAV write failed: %d", errno);
            }
            continue;
        }

        mic_get_record_status(&status);
        portENTER_CRITICAL(&s_lock);
        bool capture_idle = !s_capture_active;
        portEXIT_CRITICAL(&s_lock);
        if ((status.state == MIC_RECORD_STOPPING || !s_running) &&
            capture_idle && uxQueueMessagesWaiting(s_queue) == 0) {
            esp_err_t err = close_wav(file, data_bytes);
            file = NULL;
            if (err == ESP_OK && status.error == ESP_OK) {
                err = rename(pending, path) == 0 ? ESP_OK : ESP_FAIL;
            }
            if (err != ESP_OK || status.error != ESP_OK) {
                unlink(pending);
            }
            portENTER_CRITICAL(&s_lock);
            if (s_status.error == ESP_OK) {
                s_status.error = err;
            }
            s_status.state = s_status.error == ESP_OK ? MIC_RECORD_SAVED : MIC_RECORD_ERROR;
            if (s_status.state == MIC_RECORD_SAVED) {
                ++s_status.saved_count;
                snprintf(s_latest_wav_path, sizeof(s_latest_wav_path), "%s", path);
            }
            esp_err_t result = s_status.error;
            portEXIT_CRITICAL(&s_lock);
            if (result == ESP_OK) {
                ESP_LOGI(TAG, "saved %s (%lu bytes)", path, (unsigned long)data_bytes);
            } else {
                ESP_LOGE(TAG, "save %s failed: %s", path, esp_err_to_name(result));
            }
        }
    }

    xSemaphoreGive(s_writer_done);
    /* The owner releases the PSRAM stack with vTaskDeleteWithCaps(). */
    vTaskSuspend(NULL);
}

static bool capture_requested(void)
{
    portENTER_CRITICAL(&s_lock);
    bool recording = s_running && s_status.state == MIC_RECORDING;
    portEXIT_CRITICAL(&s_lock);
    return recording;
}

void mic_capture_task(void *arg)
{
    (void)arg;
    int32_t input[MIC_FRAME_SAMPLES];

    while (s_running) {
        /* No polling or I2S clocks while idle. Only the capture task owns enable/disable. */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (!s_running) {
            break;
        }
        bool enabled = false;
        if (capture_requested()) {
            esp_err_t err = i2s_channel_enable(s_rx_handle);
            enabled = err == ESP_OK;
            if (!enabled) {
                status_set(MIC_RECORD_STOPPING, err);
                ESP_LOGE(TAG, "I2S1 enable failed: %s", esp_err_to_name(err));
            }
        }
        bool first_block = true;
        while (enabled && capture_requested()) {
            size_t filled = 0;
            while (filled < sizeof(input) && capture_requested()) {
                size_t bytes_read = 0;
                esp_err_t err = i2s_channel_read(s_rx_handle, (uint8_t *)input + filled,
                                                sizeof(input) - filled, &bytes_read, 100);
                if (err != ESP_OK || bytes_read == 0) {
                    status_set(MIC_RECORD_STOPPING, err == ESP_OK ? ESP_ERR_TIMEOUT : err);
                    break;
                }
                filled += bytes_read;
            }
            if (!capture_requested() || filled != sizeof(input)) {
                break;
            }
            if (first_block) {
                first_block = false;
                continue;
            }

            mic_block_t block;
            for (size_t i = 0; i < MIC_FRAME_SAMPLES; ++i) {
                int64_t value = ((int64_t)(input[i] >> 8) * MIC_RECORD_GAIN) >> 8;
                if (value > INT16_MAX) {
                    value = INT16_MAX;
                } else if (value < INT16_MIN) {
                    value = INT16_MIN;
                }
                block.samples[i] = (int16_t)value;
            }
            if (capture_requested() && xQueueSend(s_queue, &block, 0) != pdTRUE) {
                status_set(MIC_RECORD_STOPPING, ESP_ERR_NO_MEM);
                ESP_LOGE(TAG, "record queue full");
            }
        }
        if (enabled) {
            esp_err_t err = i2s_channel_disable(s_rx_handle);
            if (err != ESP_OK) {
                status_set(MIC_RECORD_STOPPING, err);
            }
        }
        /* Finalization cannot pass this barrier before the final queue send. */
        portENTER_CRITICAL(&s_lock);
        s_capture_active = false;
        portEXIT_CRITICAL(&s_lock);
        xTaskNotifyGive(s_writer_task);
    }

    portENTER_CRITICAL(&s_lock);
    s_capture_active = false;
    portEXIT_CRITICAL(&s_lock);
    xTaskNotifyGive(s_writer_task);
    xSemaphoreGive(s_capture_done);
    vTaskSuspend(NULL);
}

esp_err_t mic_init(void)
{
    if (s_rx_handle != NULL) {
        return ESP_OK;
    }

    esp_err_t err = ESP_ERR_NO_MEM;
    const char *step = "record queue (PSRAM)";

    /* This project enables PSRAM only through explicit capability allocation. */
    s_queue = xQueueCreateWithCaps(MIC_QUEUE_BLOCKS, sizeof(mic_block_t),
                                   MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (s_queue == NULL) {
        goto fail;
    }
    step = "completion semaphores";
    s_capture_done = xSemaphoreCreateBinary();
    s_writer_done = xSemaphoreCreateBinary();
    if (s_capture_done == NULL || s_writer_done == NULL) {
        goto fail;
    }

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(MIC_I2S_PORT, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 8;
    chan_cfg.dma_frame_num = 128;
    step = "I2S1 RX channel";
    err = i2s_new_channel(&chan_cfg, NULL, &s_rx_handle);
    if (err != ESP_OK) {
        goto fail;
    }

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(MIC_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT,
                                                         I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = MIC_SCK_GPIO,
            .ws = MIC_WS_GPIO,
            .dout = I2S_GPIO_UNUSED,
            .din = MIC_SD_GPIO,
        },
    };
    std_cfg.slot_cfg.slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT;
    std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_LEFT;

    step = "I2S1 standard mode / DMA";
    err = i2s_channel_init_std_mode(s_rx_handle, &std_cfg);
    if (err != ESP_OK) {
        goto fail;
    }
    memset(&s_status, 0, sizeof(s_status));
    s_status.state = MIC_RECORD_IDLE;
    s_record_start_us = 0;
    s_record_end_us = 0;
    s_capture_active = false;
    s_running = true;
    step = "record tasks (PSRAM stacks)";
    err = mic_tasks_create(&s_writer_task, &s_capture_task);
    if (err != ESP_OK) {
        s_running = false;
        goto fail_tasks;
    }

    ESP_LOGI(TAG, "I2S1 microphone ready (idle, clocks off): %u Hz mono", MIC_SAMPLE_RATE);
    return ESP_OK;

fail_tasks:
    if (s_capture_task != NULL) {
        xTaskNotifyGive(s_capture_task);
        xSemaphoreTake(s_capture_done, portMAX_DELAY);
        vTaskDeleteWithCaps(s_capture_task);
        s_capture_task = NULL;
    }
    if (s_writer_task != NULL) {
        xTaskNotifyGive(s_writer_task);
        xSemaphoreTake(s_writer_done, portMAX_DELAY);
        vTaskDeleteWithCaps(s_writer_task);
        s_writer_task = NULL;
    }
fail:
    ESP_LOGE(TAG, "Init failed at %s: %s (0x%x), internal free=%u, largest=%u, PSRAM free=%u",
             step, esp_err_to_name(err), (unsigned int)err,
             (unsigned int)heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),
             (unsigned int)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),
             (unsigned int)heap_caps_get_free_size(MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (s_rx_handle != NULL) {
        i2s_del_channel(s_rx_handle);
        s_rx_handle = NULL;
    }
    if (s_queue != NULL) {
        vQueueDeleteWithCaps(s_queue);
        s_queue = NULL;
    }
    if (s_capture_done != NULL) {
        vSemaphoreDelete(s_capture_done);
        s_capture_done = NULL;
    }
    if (s_writer_done != NULL) {
        vSemaphoreDelete(s_writer_done);
        s_writer_done = NULL;
    }
    portENTER_CRITICAL(&s_lock);
    s_status.state = MIC_RECORD_ERROR;
    s_status.error = err;
    portEXIT_CRITICAL(&s_lock);
    return err;
}

void mic_deinit(void)
{
    if (s_rx_handle == NULL) {
        return;
    }

    s_running = false;
    status_set(MIC_RECORD_STOPPING, ESP_OK);
    xTaskNotifyGive(s_capture_task);
    xTaskNotifyGive(s_writer_task);
    xSemaphoreTake(s_capture_done, portMAX_DELAY);
    xSemaphoreTake(s_writer_done, portMAX_DELAY);
    vTaskDeleteWithCaps(s_capture_task);
    vTaskDeleteWithCaps(s_writer_task);
    s_capture_task = NULL;
    s_writer_task = NULL;
    i2s_del_channel(s_rx_handle);
    s_rx_handle = NULL;
    vQueueDeleteWithCaps(s_queue);
    s_queue = NULL;
    vSemaphoreDelete(s_capture_done);
    vSemaphoreDelete(s_writer_done);
    s_capture_done = NULL;
    s_writer_done = NULL;
}

esp_err_t mic_record_start(void)
{
    if (s_rx_handle == NULL || s_queue == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    portENTER_CRITICAL(&s_lock);
    bool busy = s_status.state == MIC_RECORD_STARTING ||
                s_status.state == MIC_RECORDING ||
                s_status.state == MIC_RECORD_STOPPING;
    if (!s_running || busy) {
        portEXIT_CRITICAL(&s_lock);
        return ESP_ERR_INVALID_STATE;
    }
    s_status.error = ESP_OK;
    s_status.data_bytes = 0;
    s_status.path[0] = '\0';
    s_status.state = MIC_RECORD_STARTING;
    s_record_start_us = esp_timer_get_time();
    s_record_end_us = 0;
    portEXIT_CRITICAL(&s_lock);
    xTaskNotifyGive(s_writer_task);
    return ESP_OK;
}

esp_err_t mic_record_stop(void)
{
    if (s_rx_handle == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    portENTER_CRITICAL(&s_lock);
    if (s_status.state == MIC_RECORD_STARTING || s_status.state == MIC_RECORDING) {
        s_status.state = MIC_RECORD_STOPPING;
        s_record_end_us = esp_timer_get_time();
        portEXIT_CRITICAL(&s_lock);
        xTaskNotifyGive(s_writer_task);
        return ESP_OK;
    }
    portEXIT_CRITICAL(&s_lock);
    return ESP_ERR_INVALID_STATE;
}

bool mic_get_record_status(mic_record_status_t *status)
{
    if (status == NULL) {
        return false;
    }
    portENTER_CRITICAL(&s_lock);
    *status = s_status;
    portEXIT_CRITICAL(&s_lock);
    return true;
}

uint32_t mic_get_record_elapsed_ms(void)
{
    portENTER_CRITICAL(&s_lock);
    uint64_t start = s_record_start_us;
    uint64_t end = s_record_end_us;
    mic_record_state_t state = s_status.state;
    uint32_t data_bytes = s_status.data_bytes;
    portEXIT_CRITICAL(&s_lock);

    if (state == MIC_RECORD_SAVED || state == MIC_RECORD_ERROR) {
        return (uint32_t)(((uint64_t)data_bytes * 1000U) /
                          (MIC_SAMPLE_RATE * sizeof(int16_t)));
    }
    if (start == 0 || state == MIC_RECORD_IDLE) {
        return 0;
    }
    return (uint32_t)(((end ? end : (uint64_t)esp_timer_get_time()) - start) / 1000U);
}

static int wav_newest_first(const void *a, const void *b)
{
    const mic_wav_file_t *left = a;
    const mic_wav_file_t *right = b;
    if (left->modified != right->modified) {
        return left->modified > right->modified ? -1 : 1;
    }
    return strcmp(right->name, left->name);
}

esp_err_t mic_get_wav_files(mic_wav_file_t **files, size_t *count)
{
    if (files == NULL || count == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    *files = NULL;
    *count = 0;
    if (!sdcard_is_mounted()) {
        return ESP_ERR_INVALID_STATE;
    }
    DIR *dir = opendir(MIC_WAV_DIRECTORY);
    if (dir == NULL) {
        return errno == ENOENT ? ESP_OK : ESP_FAIL;
    }

    mic_wav_file_t *items = NULL;
    size_t used = 0;
    size_t capacity = 0;
    esp_err_t err = ESP_OK;
    struct dirent *entry;
    for (;;) {
        errno = 0;
        entry = readdir(dir);
        if (entry == NULL) {
            if (errno != 0) {
                err = ESP_FAIL;
            }
            break;
        }
        size_t length = strlen(entry->d_name);
        if (length < 4 || strcasecmp(entry->d_name + length - 4, ".wav") != 0) {
            continue;
        }
        char path[sizeof(MIC_WAV_DIRECTORY) + sizeof(items[0].name)];
        snprintf(path, sizeof(path), MIC_WAV_DIRECTORY "/%s", entry->d_name);
        struct stat st;
        if (stat(path, &st) != 0 || !S_ISREG(st.st_mode)) {
            continue;
        }
        if (used == capacity) {
            size_t next = capacity + 16;
            mic_wav_file_t *grown = heap_caps_realloc(items, next * sizeof(*items),
                                                     MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
            if (grown == NULL) {
                err = ESP_ERR_NO_MEM;
                break;
            }
            items = grown;
            capacity = next;
        }
        items[used].modified = st.st_mtime;
        snprintf(items[used].name, sizeof(items[used].name), "%s", entry->d_name);
        ++used;
    }
    closedir(dir);
    if (err != ESP_OK) {
        heap_caps_free(items);
        return err;
    }
    if (used > 1) {
        qsort(items, used, sizeof(*items), wav_newest_first);
    }

    /* A newly saved unsynchronized recording must still precede dated files. */
    char latest[MIC_RECORD_PATH_MAX];
    portENTER_CRITICAL(&s_lock);
    snprintf(latest, sizeof(latest), "%s", s_latest_wav_path);
    portEXIT_CRITICAL(&s_lock);
    const char *name = strrchr(latest, '/');
    if (name != NULL) {
        for (size_t i = 1; i < used; ++i) {
            if (strcmp(items[i].name, name + 1) == 0) {
                mic_wav_file_t newest = items[i];
                memmove(items + 1, items, i * sizeof(*items));
                items[0] = newest;
                break;
            }
        }
    }
    *files = items;
    *count = used;
    return ESP_OK;
}

void mic_free_wav_files(mic_wav_file_t *files)
{
    heap_caps_free(files);
}
