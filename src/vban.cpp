/**
 * @file vban.cpp
 * @author brokulp
 * @brief implements methodes of vban library
 * @version 0.1
 * @date 2024-03-01
 * @copyright Copyright (c) 2024
 */

#include <vban.h>

/**
 * @brief method to handle an incoming frame
 * 
 * @param data pointer to data part of incoming udp packet
 * @param packetSize size of udp data packet section in bytes
 */
void vban::handlePacket(void *data, uint16_t packetSize){
    //if packet is to small for complete header abort methode
    if(packetSize < VBAN_header_size){
        return;
    }
    //format data packet
    _ptr_generalHeader_t header = (_ptr_generalHeader_t) data;
    //if packet is not vban packet abort
    if (header->FOURC != VBAN_FOURC)
    {
        return;
    }
    //get sub protocoll
    uint8_t subProtocoll = header->protocol & VBAN_MASK_PROT;

    //handle differrent subprotocolls
    switch (subProtocoll)
    {
        //handle audio packets
        case VBAN_PROTOCOL_AUDIO:
            _handleProtAudio(data,packetSize);
            break;

        case VBAN_PROTOCOL_SERIAL:
            break;

        case VBAN_PROTOCOL_SERVICE:
            break;

        case VBAN_PROTOCOL_TXT:
            break;
        
        default:
            break;
    }
    
}

/**
 * @brief methode to handle incoming audio packets
 * 
 * @param data data section of the udp packet (with vban header)
 * @param packetSize size of the vban packet in bytes (with header)
 */
void vban::_handleProtAudio(void *data, uint16_t packetSize){
    //get audio frame header
    _ptr_audioHeader_t _header = (_ptr_audioHeader_t) data;

    //abort if packet is not part of subscribed data stream
    for (uint8_t i = 0; i < 16; i++)
    {
        if (_subbedStreamName[i] != _header->streamname[i])
        {
            return;
        }
    }
    
    //decode packet information
    uint8_t _samples = _header->nbs;
    uint8_t _channels = _header->nbc;
    uint8_t _sampleRate = _header->SR & VBAN_MASK_SAMPLE_RATE;
    uint8_t _dataFormat = _header->bit & VBAN_MASK_DATA_FORMAT;
    uint8_t _dataCodec = _header->bit & VBAN_MASK_DATA_CODEC;

    //buffer for data frame
    uint16_t dataOut[VBAN_MAX_PACKET_LENGTH];

    //format data to uint16_t with zero at 32767
    switch (_dataFormat)
    {
        case VBAN_DATATYPE_BYTE8:
            int8_t* packet = (int8_t*) data;
            for (uint16_t i = VBAN_header_size; i < _samples*_channels; i++)
            {
                dataOut[i-VBAN_header_size] = (uint16_t) packet[i]*256;
            }
            break;

        case VBAN_DATATYPE_INT16:
            int16_t* packet = (int16_t*) data;
            for (uint16_t i = VBAN_header_size/2; i < _samples*_channels; i++)
            {
                dataOut[i-VBAN_header_size/2] = (uint16_t) packet[i] + 32768;
            }
            break;
            
        case VBAN_DATATYPE_INT24://currently not supported
            _postError(VBAN_ERR_UNSUPPORTED_DATATYPE);
            return;
            break;
        
        case VBAN_DATATYPE_INT32:
            int32_t* packet = (int32_t*) data;
            for (uint16_t i = VBAN_header_size/4; i < _samples*_channels; i++)
            {
                dataOut[i-VBAN_header_size/4] = (uint16_t) packet[i]/65536 + 32768;
            }
            break;

        case VBAN_DATATYPE_FLOAT32:
            float* packet = (float*) data;
            for (uint16_t i = VBAN_header_size/4; i < _samples*_channels; i++)
            {
                dataOut[i-VBAN_header_size/4] = (uint16_t) packet[i]*32767 + 32767;
            }
            break;

        case VBAN_DATATYPE_FLOAT64:
            double* packet = (double*) data;
            for (uint16_t i = VBAN_header_size/8; i < _samples*_channels; i++)
            {
                dataOut[i-VBAN_header_size/8] = (uint16_t) packet[i] * 32767 + 32767;
            }
            break;

        case VBAN_DATATYPE_12BITS://currently not supported
            _postError(VBAN_ERR_UNSUPPORTED_DATATYPE);
            return;
            break;

        case VBAN_DATATYPE_10BITS://currently not supported
            _postError(VBAN_ERR_UNSUPPORTED_DATATYPE);
            return;
            break;

        default:
            //post error code if nothing matches and abort execution
            _postError(VBAN_ERR_UNSUPPORTED_DATATYPE);
            return;
            break;
    }


    //handle codec and seperated channels
    switch (_dataCodec)
    {
        case VBAN_CODEC_PCM:
                //loop through channels
                for (uint8_t i = 0; i <= _channels; i++)
                {
                    //loop through samples
                    for (uint8_t n = 0; n <= _samples; n++)
                    {
                        _putSampleToBuffer(dataOut[i*n],i);
                    }
                }
            break;
        
        default:
            _postError(VBAN_ERR_UNSUPPORTED_CODEC);
            return;
            break;
    }
}