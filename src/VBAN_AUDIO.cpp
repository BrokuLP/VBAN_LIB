/**
 * @file VBAN_AUDIO.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "VBAN_AUDIO.h"


/**
 * @brief function to handle a audio type vban packet 
 * @note THIS DOES NOT CHECK IF THE PACKET IS OF CORRECT TYPE
 * @param packet pointer to the udp packet body
 * @param len length of the supplied body
 * @return uint8_t 
 * @retval 0 >> handler finished with success
 * @retval 1 >> codec not supported
 */
uint8_t VBAN_AUDIO::handle(void *packet, uint16_t len){
    //when this gets called we assume that we need to handle this packet
    //extract header
    ptr_audio_header_t _header = (ptr_audio_header_t)packet;

    //extract data
    uint8_t _codec = _header->RCs & VBAN_AUDIO_CODEC_MASK;
    uint8_t _dataType = _header->RCs & VBAN_AUDIO_DTYPE_MASK;
    uint16_t _numberOfChannels = _header->NBC + 1;
    uint16_t _numberOfSamples = _header->NBS +1;

    //init buffers
    int16_t _audioBuffer[_numberOfChannels][_numberOfSamples] = {0};

    //depending on codec separate samples and transform to standard data type
    switch (_codec){
        case VBAN_AUDIO_CODEC_PCM:
            
        break;
    
    default:
        return 1;
    }
    

    //when wer are finished push data to buffer
    if(!cb_put_to_buffer()){
        return 66;
    }

    //when we get here success
    return 0;
}

uint8_t VBAN_AUDIO::transform_PCM(void *input, uint16_t nbs, uint16_t nbc, uint8_t dtype, int16_t *output){
    //select extraction methode depending on datatype
    switch (dtype) {
        case VBAN_AUDIO_DTYPE_BYTE8:
        break;
    
        case VBAN_AUDIO_DTYPE_INT16:
            break;

        case VBAN_AUDIO_DTYPE_INT24:
            break;
        
        case VBAN_AUDIO_DTYPE_INT32:
            break;

        case VBAN_AUDIO_DTYPE_FLOAT32:
            break;

        case VBAN_AUDIO_DTYPE_FLOAT64:
            break;
        
        case VBAN_AUDIO_DTYPE_10BITS:
            break;
        
        case VBAN_AUDIO_DTYPE_12BITS:
            break;

    default:
        return 1;
    }

    return 0;
}


uint8_t VBAN_AUDIO::resample(uint16_t *source, uint16_t source_len, uint16_t source_freq, uint16_t *target, uint16_t target_len, uint16_t target_freq){

    //validate inputs
    if (source_len < 2) {
        //we need at least 2 sample to interpolate
        return 1;
    }
    
    //init variables
    uint16_t _timeSourceStep_us = 1000/source_freq;
    uint16_t _timeTargetStep_us = 1000/target_freq;
    uint16_t _timeElapsedSource_us = 0;
    uint16_t _timeElapsedTarget_us = 0;
    
    //check for up or downsample
    if (source_freq > target_freq){
        //downsample
    } else if (source_freq < target_freq) {
        //upsample
    } else {
        //do not resample
        if(source_len > target_len){
            return 66;
        }
        target = source;
    }
    


}