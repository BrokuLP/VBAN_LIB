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

    static constexpr uint8_t version[4] = {0 ,0 ,0 ,0};
    static constexpr uint8_t headerSize = 28;
    static constexpr uint8_t streamNameSize = 16;
    static constexpr uint32_t codeWord = 'NABV';
    static constexpr uint16_t maxNumSamples = 256;
    static constexpr uint16_t maxNumChannels = 256;
    static constexpr uint8_t IPSize = 4;
    static constexpr uint8_t GPSSize = 8;
    static constexpr uint16_t pingSize = 676;

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

    enum supportedServices{
        SERVICE_IDENT = 0,
        SERVICE_CHATUTF8 = 1,
        SERVICE_RTPACKETREGISTER = 32,
        SERVICE_RTPACKET = 33
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

    struct serviceHeader {
        unsigned int FOURC          : 32;
        unsigned int reserved0      : 5;
        unsigned int subProtocol    : 3;
        unsigned int isReply        : 1;
        unsigned int function       : 7;
        unsigned int service        : 8;
        unsigned int info           : 8;
        uint8_t StreamName[streamNameSize]; //ASCII
        unsigned int FrameCounter   : 32;
    }__attribute__((packed));
    static_assert(sizeof(serviceHeader) == headerSize, "service header must have header size");

    struct GPSPostion {
        uint8_t latitudeDegree;
        uint8_t latitudeMinute;
        uint8_t latitudeSecond;
        uint8_t latitudeScent;
        uint8_t longitudeDegree;
        uint8_t longitudeMinute;
        uint8_t longitudeSecond;
        uint8_t longitudeScent;
    }__attribute__((packed));
    static_assert(sizeof(GPSPostion) == GPSSize, "GPSPosition must have size of GPSSize");

    struct servicePing{
        serviceHeader header;
        uint32_t bitType;
        uint32_t bitFeature;
        uint32_t bitFeatureEx;
        uint32_t preferedRate;
        uint32_t minRate;
        uint32_t maxRate;
        uint32_t colorRGB;
        uint8_t  nVersion[4];
        GPSPostion GPS_Postion;
        GPSPostion USER_Position;
        uint8_t langCodeAscii[8];
        uint8_t reservedAscii[8];
        uint8_t reservedEx[64];
        uint8_t distantIPAscii[32];
        uint16_t distantPort;
        uint16_t distantReserved;
        uint8_t deviceNameAscii[64];
        uint8_t manufacturerNameAscii[64];
        uint8_t applicationNameAscii[64];
        uint8_t hostNameAscii[64];
        uint8_t userNameUtf8[128];
        uint8_t userCommentUtf8[128];
    }__attribute__((packed));
    static_assert(sizeof(servicePing) == pingSize + headerSize, "servicePing must have pingSize");

    enum langCodes{
        LANGCODE_AT = 'AT',
        LANGCODE_BE = 'BE',
        LANGCODE_BG = 'BG',
        LANGCODE_HR = 'HR',
        LANGCODE_CY = 'CY',
        LANGCODE_CZ = 'CZ',
        LANGCODE_DK = 'DK',
        LANGCODE_EE = 'EE',
        LANGCODE_FI = 'FI',
        LANGCODE_FR = 'FR',
        LANGCODE_DE = 'DE',
        LANGCODE_GR = 'GR',
        LANGCODE_HU = 'HU',
        LANGCODE_IE = 'IE',
        LANGCODE_IT = 'IT',
        LANGCODE_LV = 'LV',
        LANGCODE_LT = 'LT',
        LANGCODE_LU = 'LU',
        LANGCODE_MT = 'MT',
        LANGCODE_NL = 'NL',
        LANGCODE_PL = 'PL',
        LANGCODE_PT = 'PT',
        LANGCODE_RO = 'RO',
        LANGCODE_SK = 'SK',
        LANGCODE_SI = 'SI',
        LANGCODE_ES = 'ES',
        LANGCODE_SE = 'SE',
        LANGCODE_MX = 'MX',
        LANGCODE_CA = 'CA',
        LANGCODE_US = 'US',
    };

    enum bitType{
        BT_RECEPTOR         = 0x00000001,
        BT_TRANSMITTER      = 0x00000002,
        BT_RECEPTORSPOT     = 0x00000004,
        BT_TRANSMITTERSPOT  = 0x00000008,
        BT_VIRTUALDEVICE    = 0x00000010,        
        BT_VIRUALMIXER      = 0x00000020,        
        BT_MATRIX           = 0x00000040,
        BT_WORKSTATION      = 0x00000080,
        BT_SERVER           = 0x01000000,
    };

    enum bitFeature{
        BF_AUDIO    = 0x00000001,
        BF_AOIP     = 0x00000002,
        BF_VOIP     = 0x00000004,
        BF_SERIAL   = 0x00000100,
        BF_MIDI     = 0x00000300,
        BF_FRAME    = 0x00001000,
        BF_TXT      = 0x00010000,
    };

    enum extraBitFeature{
        EBF_UNUSED = 0,
    };
    
    /**
     * @brief function to decode an incoming UDP packet
     * 
     * @param packet        pointer to the received packet
     * @param packetSize    size of the received packet in bytes
     * @param senderIP      the IP of the sender
     * @param senderIPSize  size of the ip in bytes
     */
    returnCodes handlePacket(void *packet, uint16_t packetSize, uint8_t *senderIP, uint8_t senderIPSize);

    //configuration functions
    void setGPSPostion(GPSPostion postion);
    void setUSerPostion(GPSPostion position);
    void setLanguage(langCodes langCode);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setRate(uint32_t minRate, uint32_t prefRate, uint32_t maxRate);
    void setDeviceType(bitType type);
    void setBitFeature(bitFeature feature);
    void setExtraBitFeature(extraBitFeature extraFeature);
    void setDistant(uint8_t *distantIp, uint8_t distantIpLen, uint16_t distantPort);
    void setDeviceName(uint8_t *name, uint8_t nameLen);
    void setManufacturerName(uint8_t *name, uint8_t nameLen);
    void setApplicationName(uint8_t *name, uint8_t nameLen);
    void setHostName(uint8_t *name, uint8_t nameLen);
    void setUserName(uint8_t *name, uint8_t nameLen);
    void setUserComment(uint8_t *name, uint8_t nameLen);

private:
    struct configuration{
        GPSPostion gpsPosition;
        GPSPostion userPosition;
        langCodes langCode;
        uint8_t color[3];
        uint32_t minRate;
        uint32_t prefRate;
        uint32_t maxRate;
        bitType deviceType;
        bitFeature feature;
        extraBitFeature extraFeature;
        
    };

    //storage
    configuration sysConf;

    uint8_t subscribedStream[streamNameSize];
    uint8_t masterIP[4]; //IP to accept data from

    //callback functions
    /**
     * @brief pointer to a callback function to be supplied by the user to be able to send UDP packets
     * 
     * @param[in] packet packet contents to be send
     * @param[in] packetLen length of the packet to be send in bytes
     * @param[in] targetIp the target IP for the packet to be send to
     * @param[in] ipLen number of bytes in the IP (for IPv4 4 bytes)
     * @param[in] targetPort target port for the packet to be send to
     * @return returnCodes 
     */
    returnCodes (*callback_sendUDP)(uint8_t *packet, uint16_t packetLen, uint8_t targetIp, uint8_t ipLen, uint16_t targetPort);
    
    returnCodes (*callback_customService)(void *packet, uint16_t packetLen);

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
     * @param[in]   oldData 
     * @param[in]   packetSize 
     * @param[in]   newSamplingRate 
     * @param[out]  newData       
     * @param[in]   numChannels 
     * @param[in]   numSamples 
     */
    void convertData(void *oldData, uint16_t packetSize, uint32_t newSamplingRate, uint16_t *newData, uint16_t numChannels, uint16_t numSamples);

    /**
     * @brief function to handle service packets
     * 
     * @param[in] packet        the packet to be handled
     * @param[in] packetSize    size of the received packet
     * @return returnCodes 
     */
    returnCodes handleService(void *packet, uint16_t packetSize);

    returnCodes handleServiceIdent(void *packet, uint16_t packetSize);
    
};

#endif /* _VBAN_HPP_ */
