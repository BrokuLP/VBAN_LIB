/**
 * header file for vban library
*/
#include <stdint.h>

//definitions
#define VBAN_header_size            28
#define VBAN_FOURC                  'NABV'
#define VBAN_MASK_PROT              0b11100000
#define VBAN_MASK_SAMPLE_RATE       0b00011111
#define VBAN_MASK_DATA_FORMAT       0b11110000
#define VBAN_MASK_DATA_CODEC        0b00000111
#define VBAN_MAX_PACKET_LENGTH      1564 //in bytes

// definition of protocoll identifiers
#define VBAN_PROTOCOL_AUDIO         0x00
#define VBAN_PROTOCOL_SERIAL        0x20
#define VBAN_PROTOCOL_TXT           0x40
#define VBAN_PROTOCOL_SERVICE       0x60
#define VBAN_PROTOCOL_UNDEFINED_1   0x80
#define VBAN_PROTOCOL_UNDEFINED_2   0xA0
#define VBAN_PROTOCOL_UNDEFINED_3   0xC0
#define VBAN_PROTOCOL_USER          0xE0

//definition of codec identifiers
#define VBAN_CODEC_PCM              0x00
#define VBAN_CODEC_VBCA             0x10 //VB-AUDIO AOIP CODEC not supported
#define VBAN_CODEC_VBCV             0x20 //VB-AUDIO VOIP CODEC not supported
#define VBAN_CODEC_UNDEFINED_1      0x30
#define VBAN_CODEC_UNDEFINED_2      0x40
#define VBAN_CODEC_UNDEFINED_3      0x50
#define VBAN_CODEC_UNDEFINED_4      0x60
#define VBAN_CODEC_UNDEFINED_5      0x70
#define VBAN_CODEC_UNDEFINED_6      0x80
#define VBAN_CODEC_UNDEFINED_7      0x90
#define VBAN_CODEC_UNDEFINED_8      0xA0
#define VBAN_CODEC_UNDEFINED_9      0xB0
#define VBAN_CODEC_UNDEFINED_10     0xC0
#define VBAN_CODEC_UNDEFINED_11     0xD0
#define VBAN_CODEC_UNDEFINED_12     0xE0
#define VBAN_CODEC_USER             0xF0

//definition of data format identifiers
#define VBAN_DATATYPE_BYTE8         0x00
#define VBAN_DATATYPE_INT16         0x01
#define VBAN_DATATYPE_INT24         0x02
#define VBAN_DATATYPE_INT32         0x03
#define VBAN_DATATYPE_FLOAT32       0x04
#define VBAN_DATATYPE_FLOAT64       0x05
#define VBAN_DATATYPE_12BITS        0x06
#define VBAN_DATATYPE_10BITS        0x07

//definition of error codes
#define VBAN_ERR_UNSUPPORTED_DATATYPE     0x0001

class vban
{
private:
    //private variables
    struct _dataStreamInfo
    {
        /* data */
    };
    
    uint8_t _subbedStreamName[16];


    void _handleProtAudio(void *data, uint16_t packetSize);
    void _postError(uint16_t errorCode);

    //packet format definitions
    struct _generalHeader
    {
        uint32_t FOURC;
        uint8_t protocol;
    };
    typedef struct _generalHeader _generalHeader_t;
    typedef struct _generalHeader *_ptr_generalHeader_t;
    //header for audio frames
    struct _audioHeader
    {
        uint32_t FOURC;
        uint8_t SR;
        uint8_t nbs;
        uint8_t nbc;
        uint8_t bit;
        uint8_t streamname[16];
        uint32_t nuFrame;
    };
    typedef struct _audioHeader _audioHeader_t;
    typedef struct _audioHeader *_ptr_audioHeader_t;
    
    //callback methodes
    
public:
    void handlePacket(void* data, uint16_t packetSize);
    vban(/* args */);
    ~vban();
};

vban::vban(/* args */)
{
}

vban::~vban()
{
}
