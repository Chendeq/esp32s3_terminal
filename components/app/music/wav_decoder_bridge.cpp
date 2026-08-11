#include "wav_decoder_bridge.h"

#include "micro_wav/wav_decoder.h"

static micro_wav::WAVDecoder s_decoder;

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
