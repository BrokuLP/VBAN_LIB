/**
 * @file VBAN.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _VBAN_HPP_
#define _VBAN_HPP_

//dependencies
#include "stdint.h"

class VBAN
{
public:
    VBAN(/* args */);
    VBAN(VBAN& other) = delete;
    VBAN(VBAN&& other) = delete;
    ~VBAN();


    static constexpr uint8_t headerSize = 28;
    static constexpr uint8_t streamNameSize = 16;
    static constexpr uint32_t codeWord = 'NABV';
    static constexpr uint16_t maxNumSamples = 256;
    static constexpr uint16_t maxNumChannels = 256;
    static constexpr uint8_t IPSize = 4;

    /**
     * @brief enum with possible return codes
     * 
     */
    enum returnCodes{
        SUCCESS,
        PACKET_TOO_SMALL,
        PACKET_NOT_VBAN,
        STREAM_NOT_SUBSCRIBED,
        UNSUPPORTED_PROTOCOL,
        UNSUPPORTED_CODEC,
        INVALID_SENDER_IP,
    };

    /**
     * @brief list of supported audio data types
     * 
     */
    enum audioDataTypes{
        ADT_BYTE8 = 0,
        ADT_INT16 = 1,
        ADT_INT32 = 3,
        ADT_FLOAT32 = 4,
        ADT_FLOAT64 = 5,
    };

    /**
     * @brief definition of possible sub protocol codes
     * 
     */
    enum subProtocols{
        PROT_AUDIO      = 0,
        PROT_SERIAL     = 1,
        PROT_TXT        = 2,
        PROT_SERVICE    = 3,
        PROT_USER       = 7,
    };

    enum suportedCodecs {
        CODEC_PCM   = 0,
        CODEC_USER  = 16,
    };

    /**
     * @brief struct representing a audio header
     * @details FOURC, StreamName, FrameCounter, subProtocol are common to all headers
     */
    struct audioHeader {
        unsigned int FOURC          : 32;
        unsigned int sampleRate     : 5;
        unsigned int subProtocol    : 3;
        unsigned int nbSamples      : 8;
        unsigned int nbChannels     : 8;
        unsigned int codec          : 4;
        unsigned int reserved       : 1;
        unsigned int bitResolution  : 3;
        uint8_t StreamName[streamNameSize]; //ASCII
        unsigned int FrameCounter   : 32;
    }__attribute__((packed));
    static_assert(sizeof(audioHeader) == headerSize, "audio header must have header size");
    
    /**
     * @brief function to decode an incoming UDP packet
     * 
     * @param packet        pointer to the received packet
     * @param packetSize    size of the received packet in bytes
     * @param senderIP      the IP of the sender
     * @param senderIPSize  size of the ip in bytes
     */
    returnCodes handlePacket(void *packet, uint16_t packetSize, uint8_t *senderIP, uint8_t senderIPSize);

    

private:
    //storage

    uint8_t subscribedStream[streamNameSize];
    uint8_t masterIP[4]; //IP to accept data from

    /**
     * @brief function to handle audio packets
     * 
     * @param packet        pointer to the packet to handle
     * @param packetSize    size of the packet in byte
     * @return returnCodes  
     */
    returnCodes handleAudio(void *packet, uint16_t packetSize);

    /**
     * @brief 
     * 
     * @param oldData 
     * @param packetSize 
     * @param newSamplingRate 
     * @param newData       
     * @param numChannels 
     * @param numSamples 
     */
    void convertData(void *oldData, uint16_t packetSize, uint32_t newSamplingRate, uint16_t *newData, uint16_t numChannels, uint16_t numSamples);
    
};

#endif /* _VBAN_HPP_ */