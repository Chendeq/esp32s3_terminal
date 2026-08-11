#ifndef __WAV_DECODER_BRIDGE_H__
#define __WAV_DECODER_BRIDGE_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MUSIC_WAV_DECODER_SUCCESS = 0,
    MUSIC_WAV_DECODER_HEADER_READY = 1,
    MUSIC_WAV_DECODER_END_OF_STREAM = 2,
    MUSIC_WAV_DECODER_NEED_MORE_DATA = 3,
    MUSIC_WAV_DECODER_ERROR_NO_RIFF = -6,
    MUSIC_WAV_DECODER_ERROR_NO_WAVE = -5,
    MUSIC_WAV_DECODER_ERROR_FAILED = -4,
    MUSIC_WAV_DECODER_ERROR_UNSUPPORTED = -3,
    MUSIC_WAV_DECODER_WARNING_INVALID_INPUT = -2,
    MUSIC_WAV_DECODER_WARNING_OUTPUT_TOO_SMALL = -1,
} music_wav_decoder_result_t;

typedef enum {
    MUSIC_WAV_FORMAT_UNKNOWN = 0x0000,
    MUSIC_WAV_FORMAT_PCM = 0x0001,
    MUSIC_WAV_FORMAT_IEEE_FLOAT = 0x0003,
    MUSIC_WAV_FORMAT_ALAW = 0x0006,
    MUSIC_WAV_FORMAT_MULAW = 0x0007,
} music_wav_format_t;

void music_wav_decoder_reset(void);
int music_wav_decoder_decode(const uint8_t *input, size_t input_len, uint8_t *output,
                             size_t output_size_bytes, size_t *bytes_consumed,
                             size_t *samples_decoded);
bool music_wav_decoder_is_header_ready(void);
uint16_t music_wav_decoder_get_audio_format(void);
uint16_t music_wav_decoder_get_bits_per_sample(void);
uint8_t music_wav_decoder_get_bytes_per_output_sample(void);
uint16_t music_wav_decoder_get_channels(void);
uint32_t music_wav_decoder_get_data_chunk_size(void);
uint32_t music_wav_decoder_get_sample_rate(void);

#ifdef __cplusplus
}
#endif

#endif
