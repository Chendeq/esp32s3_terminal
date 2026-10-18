#include "music.h"

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "wav_decoder_bridge.h"
#include "app_task.h"

static const char *TAG = "music";

#define I2S_DOUT                    GPIO_NUM_39
#define I2S_BCLK                    GPIO_NUM_40
#define I2S_LRC                     GPIO_NUM_41

#define MUSIC_DIR                   "/S/MUSIC"

#define WAV_INPUT_BUF_SIZE          4096
#define PCM_BUF_SIZE                4096
#define MUSIC_LYRIC_MAX_LINES       256
#define MUSIC_LYRIC_LINE_MAX_LEN    192
#define MUSIC_PROGRESS_INTERVAL_MS  200

typedef struct {
    int nChans;
    int samprate;
    int outputSamps;
} music_frame_info_t;

typedef struct {
    uint32_t time_ms;
    char *text;
} music_lyric_entry_t;

static i2s_chan_handle_t tx_handle = NULL;

static uint8_t *wav_input_buf = NULL;
static int16_t *pcm_buf = NULL;
static FILE *current_file = NULL;
static size_t s_wav_pending_bytes = 0;

static char (*m_music_names)[MUSIC_NAME_MAX_LEN] = NULL;
static uint16_t m_music_count = 0;

static volatile int16_t m_current_index = -1;
static volatile bool m_pause_flag = false;
static volatile bool m_stop_flag = false;
static volatile bool m_is_playing = false;
static volatile uint8_t m_volume_percent = 50;

static int s_current_sample_rate = 44100;
static bool s_need_reset_decoder = false;
static uint32_t s_total_time_ms = 0;
static uint32_t s_elapsed_time_ms = 0;
static uint8_t s_progress_percent = 0;
static uint64_t s_total_samples = 0;
static uint64_t s_elapsed_samples = 0;
static TickType_t s_last_progress_tick = 0;
static int16_t s_current_lyric_index = -1;
static music_lyric_entry_t s_lyrics[MUSIC_LYRIC_MAX_LINES];
static uint16_t s_lyric_count = 0;

static pf_music_event_cb s_pf_music_handler = NULL;

QueueHandle_t g_music_cmd_queue = NULL;

static esp_err_t allocate_buffers(void);
static void free_buffers(void);
static esp_err_t ns4168_i2s_init(int sample_rate);
static void reset_decoder(void);
static void reset_progress_state(void);
static void clear_lyrics(void);
static void stop_playback(void);
static void play_song(const char *filename);
static void apply_volume(int16_t *pcm, int samples);
static bool decode_and_play_frame(void);
static void switch_to_song(int new_index);
static void set_volume_internal(uint8_t percent);
static int prepare_ns4168_output(int16_t *pcm, const music_frame_info_t *frame_info);
static void load_lyrics_for_song(const char *filename);
static void update_progress_state(size_t samples_written);
static void notify_progress_if_needed(bool force);
static int16_t find_current_lyric_index(uint32_t elapsed_ms);

static void music_send_event(music_event_id_t id)
{
    if (s_pf_music_handler) {
        s_pf_music_handler(id);
    }
}

static void reset_progress_state(void)
{
    s_total_time_ms = 0;
    s_elapsed_time_ms = 0;
    s_progress_percent = 0;
    s_total_samples = 0;
    s_elapsed_samples = 0;
    s_last_progress_tick = 0;
    s_current_lyric_index = -1;
}

static void clear_lyrics(void)
{
    for (uint16_t i = 0; i < s_lyric_count; ++i) {
        free(s_lyrics[i].text);
        s_lyrics[i].text = NULL;
        s_lyrics[i].time_ms = 0;
    }
    s_lyric_count = 0;
    s_current_lyric_index = -1;
}

static int lyric_entry_compare(const void *lhs, const void *rhs)
{
    const music_lyric_entry_t *a = (const music_lyric_entry_t *)lhs;
    const music_lyric_entry_t *b = (const music_lyric_entry_t *)rhs;
    if (a->time_ms < b->time_ms) {
        return -1;
    }
    if (a->time_ms > b->time_ms) {
        return 1;
    }
    return 0;
}

static char *trim_line(char *line)
{
    if (!line) {
        return NULL;
    }

    while (*line && isspace((unsigned char)*line)) {
        ++line;
    }

    char *end = line + strlen(line);
    while (end > line && isspace((unsigned char)end[-1])) {
        --end;
    }
    *end = '\0';
    return line;
}

static bool parse_lrc_timestamp(const char *tag, size_t len, uint32_t *time_ms)
{
    if (!tag || len < 5 || !time_ms) {
        return false;
    }

    size_t pos = 0;
    uint32_t minutes = 0;
    uint32_t seconds = 0;
    uint32_t fraction = 0;
    size_t fraction_digits = 0;

    while (pos < len && isdigit((unsigned char)tag[pos])) {
        minutes = minutes * 10U + (uint32_t)(tag[pos] - '0');
        ++pos;
    }
    if (pos >= len || tag[pos] != ':') {
        return false;
    }
    ++pos;

    if (pos + 1 >= len || !isdigit((unsigned char)tag[pos]) || !isdigit((unsigned char)tag[pos + 1])) {
        return false;
    }
    seconds = (uint32_t)(tag[pos] - '0') * 10U + (uint32_t)(tag[pos + 1] - '0');
    pos += 2;

    if (pos < len && tag[pos] == '.') {
        ++pos;
        while (pos < len && isdigit((unsigned char)tag[pos]) && fraction_digits < 3) {
            fraction = fraction * 10U + (uint32_t)(tag[pos] - '0');
            ++pos;
            ++fraction_digits;
        }
        while (pos < len && isdigit((unsigned char)tag[pos])) {
            ++pos;
        }
    }

    if (pos != len || seconds >= 60U) {
        return false;
    }

    if (fraction_digits == 1) {
        fraction *= 100U;
    } else if (fraction_digits == 2) {
        fraction *= 10U;
    }

    *time_ms = (minutes * 60U + seconds) * 1000U + fraction;
    return true;
}

static void load_lyrics_for_song(const char *filename)
{
    clear_lyrics();

    if (!filename || filename[0] == '\0') {
        return;
    }

    char lrc_name[MUSIC_NAME_MAX_LEN];
    snprintf(lrc_name, sizeof(lrc_name), "%s", filename);

    char *dot = strrchr(lrc_name, '.');
    if (dot == NULL) {
        return;
    }
    snprintf(dot, (size_t)(&lrc_name[sizeof(lrc_name) - 1] - dot) + 1U, ".lrc");

    char fullpath[MUSIC_PATH_MAX_LEN];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", MUSIC_DIR, lrc_name);

    FILE *lrc_file = fopen(fullpath, "rb");
    if (!lrc_file) {
        ESP_LOGW(TAG, "Lyric file not found: %s", fullpath);
        return;
    }

    char line_buf[256];
    while (fgets(line_buf, sizeof(line_buf), lrc_file) != NULL) {
        char *line = trim_line(line_buf);
        if (!line || line[0] != '[') {
            continue;
        }

        uint32_t timestamps[8];
        uint8_t timestamp_count = 0;
        char *cursor = line;

        while (*cursor == '[') {
            char *close = strchr(cursor, ']');
            if (!close) {
                break;
            }

            uint32_t time_ms = 0;
            if (parse_lrc_timestamp(cursor + 1, (size_t)(close - cursor - 1), &time_ms)) {
                if (timestamp_count < (sizeof(timestamps) / sizeof(timestamps[0]))) {
                    timestamps[timestamp_count++] = time_ms;
                }
            }

            cursor = close + 1;
        }

        if (timestamp_count == 0) {
            continue;
        }

        char *lyric_text = trim_line(cursor);
        if (!lyric_text || lyric_text[0] == '\0') {
            lyric_text = "";
        }

        char text_copy[MUSIC_LYRIC_LINE_MAX_LEN];
        snprintf(text_copy, sizeof(text_copy), "%s", lyric_text);

        for (uint8_t i = 0; i < timestamp_count && s_lyric_count < MUSIC_LYRIC_MAX_LINES; ++i) {
            size_t text_len = strlen(text_copy);
            char *stored_text = malloc(text_len + 1U);
            if (!stored_text) {
                ESP_LOGW(TAG, "Failed to allocate lyric text");
                fclose(lrc_file);
                qsort(s_lyrics, s_lyric_count, sizeof(s_lyrics[0]), lyric_entry_compare);
                return;
            }

            memcpy(stored_text, text_copy, text_len + 1U);
            s_lyrics[s_lyric_count].time_ms = timestamps[i];
            s_lyrics[s_lyric_count].text = stored_text;
            ++s_lyric_count;
        }
    }

    fclose(lrc_file);

    if (s_lyric_count > 1) {
        qsort(s_lyrics, s_lyric_count, sizeof(s_lyrics[0]), lyric_entry_compare);
    }

    ESP_LOGI(TAG, "Loaded %u lyric lines for %s", (unsigned int)s_lyric_count, filename);
}

static int16_t find_current_lyric_index(uint32_t elapsed_ms)
{
    int16_t current = -1;

    for (uint16_t i = 0; i < s_lyric_count; ++i) {
        if (s_lyrics[i].time_ms <= elapsed_ms) {
            current = (int16_t)i;
        } else {
            break;
        }
    }

    return current;
}

static void notify_progress_if_needed(bool force)
{
    TickType_t now = xTaskGetTickCount();
    TickType_t interval = pdMS_TO_TICKS(MUSIC_PROGRESS_INTERVAL_MS);

    if (force || s_last_progress_tick == 0 || (now - s_last_progress_tick) >= interval) {
        s_last_progress_tick = now;
        music_send_event(MUSIC_EVT_PROGRESS);
    }
}

static void update_progress_state(size_t samples_written)
{
    if (s_current_sample_rate <= 0 || samples_written == 0) {
        return;
    }

    s_elapsed_samples += (uint64_t)samples_written;
    s_elapsed_time_ms = (uint32_t)((s_elapsed_samples * 1000ULL) / (uint64_t)s_current_sample_rate);

    if (s_total_samples > 0) {
        uint64_t elapsed = s_elapsed_samples;
        if (elapsed > s_total_samples) {
            elapsed = s_total_samples;
        }

        s_total_time_ms = (uint32_t)((s_total_samples * 1000ULL) / (uint64_t)s_current_sample_rate);
        s_progress_percent = (uint8_t)((elapsed * 100ULL) / s_total_samples);
        if (s_progress_percent > 100U) {
            s_progress_percent = 100U;
        }
    } else {
        s_progress_percent = 0;
    }

    s_current_lyric_index = find_current_lyric_index(s_elapsed_time_ms);
}

static void handle_cmd_play_index(uint16_t index)
{
    if (index >= m_music_count) {
        return;
    }
    m_stop_flag = false;
    m_pause_flag = false;
    switch_to_song((int)index);
}

static void handle_cmd_prev(void)
{
    if (m_music_count == 0) {
        return;
    }
    int new_idx = (m_current_index <= 0) ? (m_music_count - 1) : (m_current_index - 1);
    handle_cmd_play_index((uint16_t)new_idx);
}

static void handle_cmd_next(void)
{
    if (m_music_count == 0) {
        return;
    }
    int new_idx = (m_current_index < 0 || m_current_index >= (int)m_music_count - 1) ? 0
                                                                                       : (m_current_index + 1);
    handle_cmd_play_index((uint16_t)new_idx);
}

static void handle_cmd_play_pause(void)
{
    if (m_is_playing) {
        m_pause_flag = !m_pause_flag;
        music_send_event(m_pause_flag ? MUSIC_EVT_PAUSED : MUSIC_EVT_RESUMED);
        ESP_LOGI(TAG, "Playback %s", m_pause_flag ? "paused" : "resumed");
    } else if (m_music_count > 0) {
        handle_cmd_play_index(m_current_index < 0 ? 0 : (uint16_t)m_current_index);
    }
}

static void handle_cmd_stop(void)
{
    m_stop_flag = true;
    m_pause_flag = false;
}

static void handle_cmd_volume(uint8_t percent)
{
    set_volume_internal(percent);
}

static void process_cmd_queue(void)
{
    music_cmd_msg_t cmd_msg;
    while (xQueueReceive(g_music_cmd_queue, &cmd_msg, 0) == pdTRUE) {
        switch (cmd_msg.cmd) {
            case MUSIC_CMD_PLAY_PAUSE:
                handle_cmd_play_pause();
                break;
            case MUSIC_CMD_PREV:
                handle_cmd_prev();
                break;
            case MUSIC_CMD_NEXT:
                handle_cmd_next();
                break;
            case MUSIC_CMD_PLAY_INDEX:
                handle_cmd_play_index(cmd_msg.data.index);
                break;
            case MUSIC_CMD_VOLUME:
                handle_cmd_volume(cmd_msg.data.volume);
                break;
            case MUSIC_CMD_STOP:
                handle_cmd_stop();
                break;
            case MUSIC_CMD_SCAN:
                music_scan_files();
                break;
            case MUSIC_CMD_PLAY_FILE:
                m_current_index = -1;
                m_stop_flag = false;
                m_pause_flag = false;
                play_song(cmd_msg.data.path);
                break;
            default:
                break;
        }
    }
}

static esp_err_t allocate_buffers(void)
{
    if (wav_input_buf == NULL) {
        wav_input_buf = heap_caps_malloc(WAV_INPUT_BUF_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
        if (!wav_input_buf) {
            ESP_LOGE(TAG, "Failed to allocate WAV input buffer");
            return ESP_ERR_NO_MEM;
        }
    }
    if (pcm_buf == NULL) {
        pcm_buf =
            heap_caps_malloc(PCM_BUF_SIZE * sizeof(int16_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
        if (!pcm_buf) {
            ESP_LOGE(TAG, "Failed to allocate PCM buffer");
            return ESP_ERR_NO_MEM;
        }
    }
    return ESP_OK;
}

static void free_buffers(void)
{
    if (wav_input_buf) {
        free(wav_input_buf);
        wav_input_buf = NULL;
    }
    if (pcm_buf) {
        free(pcm_buf);
        pcm_buf = NULL;
    }
}

static esp_err_t ns4168_i2s_init(int sample_rate)
{
    ESP_LOGI(TAG, "Initializing I2S with sample rate: %d Hz", sample_rate);

    if (tx_handle) {
        i2s_channel_disable(tx_handle);
        i2s_del_channel(tx_handle);
        tx_handle = NULL;
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    i2s_chan_config_t chan_cfg = {
        .id = I2S_NUM_0,
        .role = I2S_ROLE_MASTER,
        .dma_desc_num = 8,
        .dma_frame_num = 512,
        .auto_clear = true,
    };
    esp_err_t ret = i2s_new_channel(&chan_cfg, &tx_handle, NULL);
    if (ret != ESP_OK) {
        return ret;
    }

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate),
        .slot_cfg =
            I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg =
            {
                .mclk = I2S_GPIO_UNUSED,
                .bclk = I2S_BCLK,
                .ws = I2S_LRC,
                .dout = I2S_DOUT,
                .din = I2S_GPIO_UNUSED,
                .invert_flags =
                    {
                        .mclk_inv = false,
                        .bclk_inv = false,
                        .ws_inv = false,
                    },
            },
    };

    std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_RIGHT;

    ret = i2s_channel_init_std_mode(tx_handle, &std_cfg);
    if (ret == ESP_OK) {
        ret = i2s_channel_enable(tx_handle);
    }
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2S init failed: %s", esp_err_to_name(ret));
        i2s_del_channel(tx_handle);
        tx_handle = NULL;
    }
    return ret;
}

static void reset_decoder(void)
{
    music_wav_decoder_reset();
    s_wav_pending_bytes = 0;
    s_need_reset_decoder = false;
}

static void stop_playback(void)
{
    if (current_file) {
        fclose(current_file);
        current_file = NULL;
    }
    reset_decoder();
    free_buffers();
    reset_progress_state();
    clear_lyrics();
    m_is_playing = false;
    ESP_LOGI(TAG, "Playback stopped");
    music_send_event(MUSIC_EVT_STOPPED);
}

static void play_song(const char *filename)
{
    if (!filename || filename[0] == '\0') {
        ESP_LOGE(TAG, "Invalid filename");
        return;
    }

    stop_playback();

    if (allocate_buffers() != ESP_OK) {
        return;
    }

    char fullpath[MUSIC_PATH_MAX_LEN];
    bool absolute_path = filename[0] == '/';
    int length = absolute_path ? snprintf(fullpath, sizeof(fullpath), "%s", filename)
                               : snprintf(fullpath, sizeof(fullpath), "%s/%s", MUSIC_DIR, filename);
    if (length < 0 || (size_t)length >= sizeof(fullpath)) {
        free_buffers();
        return;
    }

    current_file = fopen(fullpath, "rb");
    if (!current_file) {
        ESP_LOGE(TAG, "Failed to open file: %s", fullpath);
        free_buffers();
        return;
    }

    reset_decoder();
    reset_progress_state();
    if (!absolute_path) {
        load_lyrics_for_song(filename);
    }

    m_is_playing = true;
    ESP_LOGI(TAG, "Started playing: %s", filename);
    music_send_event(MUSIC_EVT_STARTED);
    notify_progress_if_needed(true);
}

static void switch_to_song(int new_index)
{
    if (m_music_count == 0) {
        return;
    }
    if (new_index < 0) {
        new_index = m_music_count - 1;
    }
    if (new_index >= (int)m_music_count) {
        new_index = 0;
    }

    m_current_index = (int16_t)new_index;
    play_song(m_music_names[m_current_index]);
}

static void apply_volume(int16_t *pcm, int samples)
{
    if (m_volume_percent >= 100) {
        return;
    }
    if (m_volume_percent == 0) {
        memset(pcm, 0, (size_t)samples * sizeof(int16_t));
        return;
    }

    int scale = (m_volume_percent * 256) / 100;
    for (int i = 0; i < samples; ++i) {
        int32_t val = (int32_t)pcm[i] * scale;
        pcm[i] = (int16_t)(val >> 8);
    }
}

static int prepare_ns4168_output(int16_t *pcm, const music_frame_info_t *frame_info)
{
    if (!pcm || !frame_info || frame_info->nChans <= 0 || frame_info->outputSamps <= 0) {
        return -1;
    }
    if ((frame_info->outputSamps % frame_info->nChans) != 0) {
        return -1;
    }

    int samples_per_channel = frame_info->outputSamps / frame_info->nChans;
    if (samples_per_channel > PCM_BUF_SIZE) {
        return -1;
    }

    if (frame_info->nChans == 1) {
        return samples_per_channel;
    }

    if (frame_info->nChans == 2) {
        for (int i = 0; i < samples_per_channel; ++i) {
            int32_t mixed = (int32_t)pcm[2 * i] + (int32_t)pcm[2 * i + 1];
            pcm[i] = (int16_t)(mixed / 2);
        }
        return samples_per_channel;
    }

    return -1;
}

static bool decode_and_play_frame(void)
{
    if (!current_file) {
        return true;
    }

    if (s_wav_pending_bytes >= WAV_INPUT_BUF_SIZE) {
        ESP_LOGE(TAG, "WAV pending buffer overflow");
        return false;
    }

    size_t bytes_read =
        fread(wav_input_buf + s_wav_pending_bytes, 1, WAV_INPUT_BUF_SIZE - s_wav_pending_bytes,
              current_file);
    size_t total_bytes = s_wav_pending_bytes + bytes_read;
    if (total_bytes == 0) {
        return feof(current_file) ? false : true;
    }

    const uint8_t *input = wav_input_buf;
    size_t input_left = total_bytes;
    bool reached_eof = feof(current_file);

    while (input_left > 0) {
        size_t bytes_consumed = 0;
        size_t samples_decoded = 0;
        int result = music_wav_decoder_decode(input, input_left, (uint8_t *)pcm_buf,
                                              PCM_BUF_SIZE * sizeof(int16_t), &bytes_consumed,
                                              &samples_decoded);

        input += bytes_consumed;
        input_left -= bytes_consumed;

        if (result == MUSIC_WAV_DECODER_HEADER_READY) {
            uint32_t sample_rate = music_wav_decoder_get_sample_rate();
            uint16_t channels = music_wav_decoder_get_channels();
            uint16_t bits_per_sample = music_wav_decoder_get_bits_per_sample();
            uint8_t bytes_per_sample = music_wav_decoder_get_bytes_per_output_sample();
            uint32_t data_chunk_size = music_wav_decoder_get_data_chunk_size();

            ESP_LOGI(TAG, "WAV header: %lu Hz, %u ch, %u bit",
                     (unsigned long)sample_rate, (unsigned int)channels,
                     (unsigned int)bits_per_sample);

            if (sample_rate < 8000 || sample_rate > 96000 || bits_per_sample != 16 ||
                (channels != 1 && channels != 2)) {
                ESP_LOGE(TAG, "Unsupported WAV format: expected 8-96kHz/16-bit mono or stereo");
                return false;
            }

            if (tx_handle == NULL || (int)sample_rate != s_current_sample_rate) {
                if (ns4168_i2s_init((int)sample_rate) != ESP_OK) {
                    return false;
                }
                s_current_sample_rate = (int)sample_rate;
            }

            if (channels > 0 && bytes_per_sample > 0 && data_chunk_size != UINT32_MAX) {
                uint16_t format = music_wav_decoder_get_audio_format();
                uint32_t encoded_bytes = (format == MUSIC_WAV_FORMAT_ALAW ||
                                          format == MUSIC_WAV_FORMAT_MULAW) ? 1 : bytes_per_sample;
                s_total_samples = data_chunk_size / ((uint32_t)channels * encoded_bytes);
                s_total_time_ms =
                    (uint32_t)((s_total_samples * 1000ULL) / (uint64_t)s_current_sample_rate);
            }

            s_current_lyric_index = find_current_lyric_index(0);
            notify_progress_if_needed(true);
            continue;
        }

        if (result == MUSIC_WAV_DECODER_SUCCESS) {
            music_frame_info_t frame_info = {
                .nChans = (int)music_wav_decoder_get_channels(),
                .samprate = (int)music_wav_decoder_get_sample_rate(),
                .outputSamps = (int)samples_decoded,
            };

            int samples_to_write = prepare_ns4168_output(pcm_buf, &frame_info);
            if (samples_to_write <= 0) {
                ESP_LOGE(TAG, "Invalid WAV frame layout");
                return false;
            }

            apply_volume(pcm_buf, samples_to_write);

            size_t bytes_written = 0;
            uint32_t timeout_ms = (uint32_t)samples_to_write * 1000U /
                                   (uint32_t)s_current_sample_rate + 100U;
            esp_err_t ret =
                i2s_channel_write(tx_handle, pcm_buf, (size_t)samples_to_write * sizeof(int16_t),
                                  &bytes_written, timeout_ms);
            if (ret != ESP_OK || bytes_written != (size_t)samples_to_write * sizeof(int16_t)) {
                ESP_LOGE(TAG, "Failed to write PCM to I2S");
                return false;
            }

            update_progress_state((size_t)samples_to_write);
            notify_progress_if_needed(false);
            continue;
        }

        if (result == MUSIC_WAV_DECODER_NEED_MORE_DATA) {
            break;
        }

        if (result == MUSIC_WAV_DECODER_END_OF_STREAM) {
            update_progress_state((size_t)0);
            notify_progress_if_needed(true);
            return false;
        }

        ESP_LOGE(TAG, "WAV decode failed: %d", result);
        return false;
    }

    if (input_left > 0) {
        memmove(wav_input_buf, input, input_left);
    }
    s_wav_pending_bytes = input_left;

    if (reached_eof && s_wav_pending_bytes == 0) {
        notify_progress_if_needed(true);
        return false;
    }

    return true;
}

static void set_volume_internal(uint8_t percent)
{
    if (percent > 100) {
        percent = 100;
    }
    m_volume_percent = percent;
    ESP_LOGI(TAG, "Volume set to %u%%", (unsigned int)percent);
    music_send_event(MUSIC_EVT_VOLUME_CHANGED);
}

esp_err_t music_init(void)
{
    ESP_LOGI(TAG, "Initializing music player...");
    esp_err_t ret = ns4168_i2s_init(44100);
    if (ret != ESP_OK) {
        return ret;
    }

    reset_decoder();
    reset_progress_state();
    clear_lyrics();

    if (g_music_cmd_queue == NULL) {
        g_music_cmd_queue = xQueueCreate(5, sizeof(music_cmd_msg_t));
        if (!g_music_cmd_queue) {
            ESP_LOGE(TAG, "Failed to create music event queue");
            return ESP_ERR_NO_MEM;
        }
    }
    return ESP_OK;
}

void music_scan_files(void)
{
    if (!m_music_names) {
        m_music_names = (char (*)[MUSIC_NAME_MAX_LEN])heap_caps_malloc(
            MUSIC_MAX_FILES * sizeof(*m_music_names), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
        if (!m_music_names) {
            return;
        }
    }

    DIR *dir = opendir(MUSIC_DIR);
    if (!dir) {
        ESP_LOGE(TAG, "Failed to open directory: %s", MUSIC_DIR);
        m_music_count = 0;
        return;
    }

    struct dirent *entry;
    m_music_count = 0;
    while ((entry = readdir(dir)) != NULL && m_music_count < MUSIC_MAX_FILES) {
        if (entry->d_type == DT_REG) {
            const char *name = entry->d_name;
            int len = (int)strlen(name);
            if (len > 4 && strcasecmp(name + len - 4, ".wav") == 0) {
                snprintf(m_music_names[m_music_count], MUSIC_NAME_MAX_LEN, "%s", name);
                ++m_music_count;
            }
        }
    }
    closedir(dir);

    ESP_LOGI(TAG, "Found %u WAV files", (unsigned int)m_music_count);

    if (m_current_index >= (int16_t)m_music_count) {
        m_current_index = -1;
    }
    music_send_event(MUSIC_EVT_SCAN_DONE);
}

uint16_t music_get_count(void)
{
    return m_music_count;
}

const char *music_get_name(uint16_t index)
{
    if (!m_music_names || index >= m_music_count) {
        return NULL;
    }
    return m_music_names[index];
}

int16_t music_get_current_index(void)
{
    return m_current_index;
}

const char *music_get_current_name(void)
{
    if (!m_music_names || m_current_index < 0 || m_current_index >= (int16_t)m_music_count) {
        return NULL;
    }
    return m_music_names[m_current_index];
}

bool music_is_paused(void)
{
    return m_pause_flag;
}

bool music_is_playing(void)
{
    return m_is_playing;
}

uint32_t music_get_total_time_ms(void)
{
    return s_total_time_ms;
}

uint32_t music_get_elapsed_time_ms(void)
{
    return s_elapsed_time_ms;
}

uint8_t music_get_progress_percent(void)
{
    return s_progress_percent;
}

uint16_t music_get_lyric_count(void)
{
    return s_lyric_count;
}

int16_t music_get_current_lyric_index(void)
{
    return s_current_lyric_index;
}

const char *music_get_lyric_line(uint16_t index)
{
    if (index >= s_lyric_count) {
        return NULL;
    }
    return s_lyrics[index].text;
}

void music_set_volume(uint8_t percent)
{
    set_volume_internal(percent);
}

uint8_t music_get_volume(void)
{
    return m_volume_percent;
}

void music_register_handler(pf_music_event_cb handler)
{
    s_pf_music_handler = handler;
}

BaseType_t music_send_cmd(const music_cmd_msg_t *msg)
{
    if (g_music_cmd_queue == NULL) {
        return pdFALSE;
    }
    BaseType_t result = xQueueSendToBack(g_music_cmd_queue, msg, pdMS_TO_TICKS(10));
    if (result == pdTRUE) {
        music_task_start();
    }
    return result;
}

void music_play(void)
{
    process_cmd_queue();

    if (s_need_reset_decoder) {
        reset_decoder();
    }

    if (m_stop_flag) {
        m_stop_flag = false;
        m_pause_flag = false;
        stop_playback();
        return;
    }

    if (m_pause_flag) {
        vTaskDelay(pdMS_TO_TICKS(20));
        return;
    }

    if (m_is_playing && current_file) {
        bool success = decode_and_play_frame();
        if (!success) {
            music_send_event(MUSIC_EVT_SONG_END);
            if (m_music_count > 0 && m_current_index >= 0) {
                int next = (m_current_index + 1) % m_music_count;
                switch_to_song(next);
            } else {
                stop_playback();
            }
        }
    } else {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
