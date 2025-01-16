#include "VBAN_LIB.h"

/**
 * @brief main function of the library, checks if a packet is vban and if true handles it
 * 
 * @param packet pointer to a udp packet
 * @param len size of packet in bytes
 * @return uint8_t 
 * @retval 0 >> packet was vban hand has been handled
 * @retval 1 >> packet not large enough for vban header
 * @retval 2 >> FORC indicates no VBAN packet
 */
uint8_t VBAN_LIB::handlePacket(void *packet, uint16_t len){
    //check if packet can even contain a header
    if (len < VBAN_HEADER_LEN){
        return 1;
    }
    //decode header
    ptr_audio_header_t _header = (ptr_audio_header_t)packet;

    //check if packet is VBAN
    if (_header->FORC != 'NABV'){
        return 2;
    }

}