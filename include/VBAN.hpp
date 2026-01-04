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
    VBAN(uint8_t *ManufacturerName, uint8_t ManufacturerNameLen);
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

    static constexpr uint8_t maxDeviceNameLen = 64;
    static constexpr uint8_t maxManufacturerNameLen = 64;
    static constexpr uint8_t maxApplicationNameLen = 64;
    static constexpr uint8_t maxHostNameLen = 64;
    static constexpr uint8_t maxUserNameLen = 128;
    static constexpr uint8_t maxUserCommentLen = 128;
    static constexpr uint8_t maxDistantIpLen = 32;

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
        INVALID_PARAMETERS,
        NOT_IPV4,
        BUFFER_TOO_SMALL,
    };

    /**
     * @brief list of supported audio data types
     * 
     */
    enum audioDataTypes{
        ADT_BYTE8 = 0x00,
        ADT_INT16 = 0x01,
        ADT_INT24 = 0x02,
        ADT_INT32 = 0x03,
        ADT_FLOAT32 = 0x04,
        ADT_FLOAT64 = 0x05,
        ADT_12BITS = 0x06,
        ADT_10BITS = 0x07,
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

    struct GPSPosition {
        uint8_t latitudeDegree;
        uint8_t latitudeMinute;
        uint8_t latitudeSecond;
        uint8_t latitudeScent;
        uint8_t longitudeDegree;
        uint8_t longitudeMinute;
        uint8_t longitudeSecond;
        uint8_t longitudeScent;
    }__attribute__((packed));
    static_assert(sizeof(GPSPosition) == GPSSize, "GPSPosition must have size of GPSSize");

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
        GPSPosition GPS_Postion;
        GPSPosition USER_Position;
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

    struct rgbColor{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    enum langCodes{
        LANGCODE_BG = 'BG', // Bulgarian (BG)
        LANGCODE_CS = 'CS', // Czech (CS)
        LANGCODE_DA = 'DA', // Danish (DA)
        LANGCODE_DE = 'DE', // German (DE)
        LANGCODE_EL = 'EL', // Greek (EL)
        LANGCODE_EN = 'EN', // English (EN)
        LANGCODE_ES = 'ES', // Spanish (ES)
        LANGCODE_ET = 'ET', // Estonian (ET)
        LANGCODE_FI = 'FI', // Finnish (FI)
        LANGCODE_FR = 'FR', // French (FR)
        LANGCODE_HR = 'HR', // Croatian (HR)
        LANGCODE_HU = 'HU', // Hungarian (HU)
        LANGCODE_IT = 'IT', // Italian (IT)
        LANGCODE_LB = 'LB', // Luxembourgish (LB)
        LANGCODE_LT = 'LT', // Lithuanian (LT)
        LANGCODE_LV = 'LV', // Latvian (LV)
        LANGCODE_MT = 'MT', // Maltese (MT)
        LANGCODE_NL = 'NL', // Dutch (NL)
        LANGCODE_PL = 'PL', // Polish (PL)
        LANGCODE_PT = 'PT', // Portuguese (PT)
        LANGCODE_RO = 'RO', // Romanian (RO)
        LANGCODE_SK = 'SK', // Slovak (SK)
        LANGCODE_SL = 'SL', // Slovenian (SL)
        LANGCODE_SV = 'SV', // Swedish (SV)
        LANGCODE_NO = 'NO', // Norwegian (NO) - general
        LANGCODE_NB = 'NB', // Norwegian Bokmål (NB)
        LANGCODE_NN = 'NN', // Norwegian Nynorsk (NN)
        LANGCODE_SE = 'SE', // Northern Sámi (SE)
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

    /**
     * @brief function to set the GPS position of the device
     * 
     * @param[in] postion the new gps position
     */
    void setGPSPosition(GPSPosition postion);

    /**
     * @brief function to set a user define gps position
     * 
     * @param[in] position a new user defined position 
     */
    void setUSerPostion(GPSPosition position);

    /**
     * @brief Set the Language object
     * 
     * @param langCode new language code of the device
     */
    void setLanguage(langCodes langCode);

    /**
     * @brief Set the Color object
     * 
     * @param red 
     * @param green 
     * @param blue 
     */
    void setColor(uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Set the Rate object
     * 
     * @param minRate 
     * @param prefRate 
     * @param maxRate 
     * @return returnCodes 
     */
    returnCodes setRate(uint32_t minRate, uint32_t prefRate, uint32_t maxRate);

    /**
     * @brief Set the Device Type object
     * 
     * @param type 
     */
    void setDeviceType(bitType type);

    /**
     * @brief Set the Bit Feature object
     * 
     * @param feature 
     */
    void setBitFeature(bitFeature feature);

    /**
     * @brief Set the Extra Bit Feature object
     * 
     * @param extraFeature 
     */
    void setExtraBitFeature(extraBitFeature extraFeature);

    /**
     * @brief not exactly clear what this does in the protocol
     * 
     * @param distantIp 
     * @param distantIpLen 
     * @param distantPort 
     * @return returnCodes 
     */
    returnCodes setDistant(uint8_t *distantIp, uint8_t distantIpLen, uint16_t distantPort);

    /**
     * @brief Set the device name
     * 
     * @param name new device name (ASCII)
     * @param nameLen length of new device name
     */
    void setDeviceName(uint8_t *name, uint8_t nameLen);

    /**
     * @brief Set the name of the manufacturer
     * 
     * @param name new manufacturer name (ASCII)
     * @param nameLen length of the manufacture name
     */
    void setManufacturerName(uint8_t *name, uint8_t nameLen);

    /**
     * @brief Set the Application name
     * 
     * @param name new application name (ASCII)
     * @param nameLen length of th application name
     */
    void setApplicationName(uint8_t *name, uint8_t nameLen);

    /**
     * @brief Set the host name of the device
     * 
     * @param name host name (ASCII)
     * @param nameLen length of the host name
     */
    void setHostName(uint8_t *name, uint8_t nameLen);

    /**
     * @brief Set the User Name object (UTF8)
     * 
     * @param name new user name (UTF8)
     * @param nameLen length of the user name
     */
    void setUserName(uint8_t *name, uint8_t nameLen);

    /**
     * @brief Set the User Comment object (UTF8)
     * 
     * @param name new user comment (UTF8)
     * @param nameLen length of the user comment
     */
    void setUserComment(uint8_t *comment, uint8_t commentLen);

private:
    struct configuration{
        GPSPosition gpsPosition;
        GPSPosition userPosition;
        langCodes langCode;
        rgbColor color;
        uint32_t minRate;
        uint32_t prefRate;
        uint32_t maxRate;
        bitType deviceType;
        bitFeature feature;
        extraBitFeature extraFeature;
        uint8_t deviceName[maxDeviceNameLen];
        uint8_t manufacturerName[maxManufacturerNameLen];
        uint8_t applicationName[maxApplicationNameLen];
        uint8_t hostName[maxHostNameLen];
        uint8_t userName[maxUserNameLen]; //utf8
        uint8_t userComment[maxUserCommentLen]; //utf8
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
    returnCodes (*callback_sendUDP)(uint8_t *packet, uint16_t packetLen, uint8_t *targetIp, uint8_t ipLen, uint16_t targetPort);
    
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

    /**
     * @brief function to handle identification service request
     * 
     * @param[in] packet 
     * @param[in] packetSize 
     * @return returnCodes 
     */
    returnCodes handleServiceIdent(void *packet, uint16_t packetSize);

    /**
     * @brief sends a ping0 to a target ip address and port
     * 
     * @param[in] targetIp ip as uint8_t array
     * @param[in] targetIpLen size of ip address (4 for IPv4)
     * @param[in] targetPort  port to send packet to
     * @return returnCodes 
     */
    returnCodes sendPing0(uint8_t *targetIp, uint8_t targetIpLen, uint16_t targetPort);

    /**
     * @brief function to copy data from one position struct ot another
     * 
     * @param[in] source 
     * @param[in] target 
     */
    void copyPosition(GPSPosition source, GPSPosition *target);

    /**
     * @brief function to replace a none ASCII character with '?'     * 
     * @param c character to be checked
     * @return uint8_t either c if c is ASCII else '?'
     */
    uint8_t VBAN::replaceNoneAsciiChar(uint8_t c);

    /**
     * @brief function to decode data from a PCM stream to INT16 format
     * 
     * @param[in] data 
     * @param[in] dataLen number of total samples in input data
     * @param[in] dataType type of data
     * @param[out] result decoded data as INT16
     * @param[in] resultLen size of result buffer in samples
     */
    returnCodes decodePCM(uint8_t *data, uint16_t dataLen, audioDataTypes dataType, int16_t *result, uint16_t resultLen);
    
};

#endif /* _VBAN_HPP_ */
