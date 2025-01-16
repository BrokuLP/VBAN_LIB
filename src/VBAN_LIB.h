/**
 * @file VBA_LIB.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef VBAN_LIB_H
#define VBAN_LIB_H

//dependencies
#include <stdint.h>
#include <VBAN_AUDIO.h>
#include <VBAN_SERIAL.h>
#include <VBAN_SERVICE.h>
#include <vban_TEST.h>


//macros
#define VBAN_HEADER_LEN 28
#define VBAN_STREAM_NAME_LEN 16

class VBAN_LIB
{
private:
    //structs
    struct audio_header{
        uint32_t FORC;
        uint8_t SR;
        uint8_t NBS;
        uint8_t NBC;
        uint8_t CODEC;
        uint8_t name[VBAN_STREAM_NAME_LEN];
        uint32_t RCs;
    }__attribute__((packed));

    //typedefs
    typedef struct audio_header audio_header_t;
    typedef struct audio_header *ptr_audio_header_t;
public:
    VBAN_LIB(/* args */);
    
    uint8_t handlePacket(void *packet, uint16_t len);
    ~VBAN_LIB();
};

VBAN_LIB::VBAN_LIB(/* args */)
{
}

VBAN_LIB::~VBAN_LIB()
{
}

#endif