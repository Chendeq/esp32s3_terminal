#include "wav_decoder_bridge.h"
#include "music.h"

#include <cstdio>

#include "micro_wav/wav_decoder.h"

static micro_wav::WAVDecoder s_decoder;

extern "C" bool music_get_file_duration_ms(const char *path, uint32_t *duration_ms)
{
    if (path == nullptr || duration_ms == nullptr) {
        return false;
    }
    FILE *file = fopen(path, "rb");
    if (file == nullptr) {
        return false;
    }

    micro_wav::WAVDecoder decoder;
    decoder.reset();
    uint8_t input[256];
    bool ready = false;
    size_t size;
    while ((size = fread(input, 1, sizeof(input), file)) > 0) {
        size_t consumed = 0;
        size_t samples = 0;
        auto result = decoder.decode(input, size, nullptr, 0, consumed, samples);
        if (result == micro_wav::WAV_DECODER_HEADER_READY) {
            uint32_t rate = decoder.get_sample_rate();
            uint16_t channels = decoder.get_channels();
            if (rate < 8000 || rate > 96000 || (channels != 1 && channels != 2) ||
                decoder.get_bits_per_sample() != 16) {
                break;
            }
            long data_start = ftell(file) - (long)(size - consumed);
            if (data_start < 0 || fseek(file, 0, SEEK_END) != 0) {
                break;
            }
            long file_end = ftell(file);
            if (file_end < data_start) {
                break;
            }
            uint64_t data_size = (uint64_t)(file_end - data_start);
            uint32_t declared_size = decoder.get_data_chunk_size();
            if (declared_size != UINT32_MAX && data_size > declared_size) {
                data_size = declared_size;
            }
            auto format = decoder.get_audio_format();
            uint32_t sample_bytes = (format == micro_wav::WAV_FORMAT_ALAW ||
                                     format == micro_wav::WAV_FORMAT_MULAW) ? 1 : 2;
            uint64_t duration = (data_size / (channels * sample_bytes)) * 1000U / rate;
            *duration_ms = duration > UINT32_MAX ? UINT32_MAX : (uint32_t)duration;
            ready = true;
            break;
        }
        if (result != micro_wav::WAV_DECODER_NEED_MORE_DATA || consumed != size) {
            break;
        }
    }
    fclose(file);
    return ready;
}

extern "C" void music_wav_decoder_reset(void)
{
    s_decoder.reset();
}

extern "C" int music_wav_decoder_decode(const uint8_t *input, size_t input_len, uint8_t *output,
                                        size_t output_size_bytes, size_t *bytes_consumed,
                                        size_t *samples_decoded)
{
    size_t local_bytes_consumed = 0;
    size_t local_samples_decoded = 0;

    micro_wav::WAVDecoderResult result =
        s_decoder.decode(input, input_len, output, output_size_bytes, local_bytes_consumed,
                         local_samples_decoded);

    if (bytes_consumed) {
        *bytes_consumed = local_bytes_consumed;
    }
    if (samples_decoded) {
        *samples_decoded = local_samples_decoded;
    }

    return (int)result;
}

extern "C" bool music_wav_decoder_is_header_ready(void)
{
    return s_decoder.is_header_ready();
}

extern "C" uint16_t music_wav_decoder_get_audio_format(void)
{
    return (uint16_t)s_decoder.get_audio_format();
}

extern "C" uint16_t music_wav_decoder_get_bits_per_sample(void)
{
    return s_decoder.get_bits_per_sample();
}

extern "C" uint8_t music_wav_decoder_get_bytes_per_output_sample(void)
{
    return s_decoder.get_bytes_per_output_sample();
}

extern "C" uint16_t music_wav_decoder_get_channels(void)
{
    return s_decoder.get_channels();
}

extern "C" uint32_t music_wav_decoder_get_data_chunk_size(void)
{
    return s_decoder.get_data_chunk_size();
}

extern "C" uint32_t music_wav_decoder_get_sample_rate(void)
{
    return s_decoder.get_sample_rate();
}
