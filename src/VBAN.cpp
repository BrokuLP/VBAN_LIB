#include "VBAN.hpp"

VBAN::VBAN(){

}

VBAN::returnCodes VBAN::handlePacket(void *packet, uint16_t packetSize, uint8_t *senderIP, uint8_t senderIPsize){
    
    //check IP
    if (senderIPsize != IPSize){
        return INVALID_SENDER_IP;
    }
    for (uint8_t i = 0; i < senderIPsize; i++){
        if (senderIP[i] != masterIP[i]){
            return INVALID_SENDER_IP;
        }
    }

    //check if packet is big enough
    if (packetSize < headerSize) {
        return PACKET_TOO_SMALL;
    }

    //extract header
    audioHeader *_header_ptr = reinterpret_cast <audioHeader*> (packet);

    //check if packet is VBAN
    if (_header_ptr->FOURC != codeWord) {
        return PACKET_NOT_VBAN;
    }

    //check if stream is subscribed
    for (uint8_t i = 0; i < streamNameSize; i++) {
        if(subscribedStream[i] != _header_ptr->StreamName[i]){
            return STREAM_NOT_SUBSCRIBED;
        }
    }

    switch (_header_ptr->subProtocol){
        case PROT_AUDIO:
            return handleAudio(packet, packetSize);
        
        default:
            return UNSUPPORTED_PROTOCOL;
    }
    
}

VBAN::returnCodes VBAN::handleAudio(void *packet, uint16_t packetSize){
    //decode header
    audioHeader *_header_ptr = reinterpret_cast <audioHeader*> (packet);

    uint16_t _numSamples = _header_ptr->nbChannels*_header_ptr->nbSamples;

    //select correct codec
    switch (_header_ptr->codec)
    {
    case CODEC_PCM:
        switch (_header_ptr->bitResolution) {
            case ADT_BYTE8:
                /* code */
                break;
            
            default:
                break;
            }
        break;

    case CODEC_USER:
        break;
        
    default:
        return UNSUPPORTED_CODEC;
    }
}

void VBAN::convertData(void *oldData, uint16_t packetSize, uint32_t newSamplingRate, uint16_t *newData, uint16_t numChannels, uint16_t numSamples){

    //unpack header
    audioHeader* _header_ptr = reinterpret_cast <audioHeader*> (oldData);

    //get required data


}