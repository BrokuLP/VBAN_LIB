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
#include <VBAN_TEXT.h>

//for now enable all sub modules
#define VBAN_SUPPORT_AUDIO
#define VBAN_SUPPORT_TEXT
#define VBAN_SUPPORT_SERVICE
#define VBAN_SUPPORT_SERIAL

//macros
#define VBAN_HEADER_LEN         28
#define VBAN_STREAM_NAME_LEN    16

//bit masks
#define VBAN_SUBPROTOCOL_MASK   0b11100000

//protocol indexes
#define VBAN_PROT_SERIAL        0x20
#define VBAN_PROT_AUDIO         0x00
#define VBAN_PROT_TEXT          0x40
#define VBAN_PROT_SERVICE       0x60
#define VBAN_PROT_USER          0xE0

class VBAN_LIB
{
private:
    //structs
    struct header{
        uint32_t FORC;
        uint8_t SR;
        uint8_t NBS;
        uint8_t NBC;
        uint8_t CODEC;
        uint8_t name[VBAN_STREAM_NAME_LEN];
        uint32_t RCs;
    }__attribute__((packed));

    //typedefs
    typedef struct header header_t;
    typedef struct header *ptr_header_t;

    //class instances
    #ifdef VBAN_SUPPORT_AUDIO
        VBAN_AUDIO audio;
    #endif

public:
    VBAN_LIB(/* args */);
    
    uint8_t handlePacket(void *packet, uint16_t len);
    ~VBAN_LIB();
};

/**
 * @brief init function for vban library
 * 
 */
VBAN_LIB::VBAN_LIB(/* args */)
{
    
    //if audio is supported init audio handler
    #ifdef VBAN_SUPPORT_AUDIO
        audio.init();
    #endif
}

VBAN_LIB::~VBAN_LIB()
{
}

#endif