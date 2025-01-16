/**
 * @file VBAN_AUDIO.h
 * @author your name (you@domain.com)
 * @brief header file for the audio sub protocol handler
 * @version 0.1
 * @date 2025-01-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdint.h>




class VBAN_AUDIO
{
private:
    bool (*cb_put_to_buffer)(uint16_t *, uint8_t);
    uint16_t requestedSampleRate;

    //structs
    struct audio_header{
        uint32_t FORC;
        uint8_t SR;
        uint8_t NBS;
        uint8_t NBC;
        uint8_t CODEC;
        uint8_t name[16];
        uint32_t RCs;
    }__attribute__((packed));

    //typedefs
    typedef struct audio_header audio_header_t;
    typedef struct audio_header *ptr_audio_header_t;

    //private functions
    uint8_t resample(uint16_t *source, uint16_t source_len, uint16_t source_freq, uint16_t *target, uint16_t target_len, uint16_t target_freq);
public:
    //public functions
    uint8_t init();
    uint8_t handle(void *packet, uint16_t len);
    VBAN_AUDIO(/* args */);
    ~VBAN_AUDIO();
};

VBAN_AUDIO::VBAN_AUDIO(/* args */)
{
}

VBAN_AUDIO::~VBAN_AUDIO()
{
}
