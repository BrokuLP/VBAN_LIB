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
    uint8_t subProtocoll = VBAN_MASK_PROT & header->protocol;

    //handle differrent subprotocolls
    switch (subProtocoll)
    {
    case VBAN_PROT_AUDIO:
        break;
    
    default:
        break;
    }
    
}