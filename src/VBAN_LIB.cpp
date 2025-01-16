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
 * @retval 3 >> protocol not supported
 */
uint8_t VBAN_LIB::handlePacket(void *packet, uint16_t len){
    //check if packet can even contain a header
    if (len < VBAN_HEADER_LEN){
        return 1;
    }
    //decode header
    ptr_header_t _header = (ptr_header_t)packet;

    //check if packet is VBAN
    if (_header->FORC != 'NABV'){
        return 2;
    }

    //handle packet according to subprotocol
    switch (_header->SR & VBAN_SUBPROTOCOL_MASK){
    
    #ifdef VBAN_SUPPORT_AUDIO
        case VBAN_PROT_AUDIO:
            if (0 != audio.handle(packet, len)){
                return 66;
            }
        break;
    #endif

    #ifdef VBAN_SUPPORT_TEXT
        case VBAN_PROT_TEXT:

        break;
    #endif

    #ifdef VBAN_SUPPORT_SERVICE
        case VBAN_PROT_SERVICE:

        break;
    #endif
    #ifdef VBAN_SUPPORT_SERIAL
        case VBAN_PROT_SERIAL:

        break;
    #endif

        default:
            return 3;
    }


}