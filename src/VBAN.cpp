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

VBAN::returnCodes VBAN::handleService(void *packet, uint16_t packetSize){
    serviceHeader *_header_ptr = reinterpret_cast <serviceHeader*>(packet);

    switch (_header_ptr->service){
        case SERVICE_IDENT:
            break;

        case SERVICE_CHATUTF8:
            break;
        
        case SERVICE_RTPACKETREGISTER:
            break;
        
        case SERVICE_RTPACKET:
            break;
    
        default:
            //if nothing matches, assume custom service
            return callback_customService(packet, packetSize);
    }

    return SUCCESS;
}

VBAN::returnCodes VBAN::handleServiceIdent(void *packet, uint16_t packetSize) {
    serviceHeader *_header_ptr = reinterpret_cast <serviceHeader*>(packet);

    //handle ident request
    if (!_header_ptr->isReply){
        uint32_t _requestId = _header_ptr->FrameCounter;
    }

    return SUCCESS;
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


void VBAN::setGPSPostion(GPSPostion postion){
    sysConf
}
void VBAN::setUSerPostion(GPSPostion position);
void VBAN::setLanguage(langCodes langCode);
void VBAN::setColor(uint8_t red, uint8_t green, uint8_t blue);
void VBAN::setRate(uint32_t minRate, uint32_t prefRate, uint32_t maxRate);
void VBAN::setDeviceType(bitType type);
void VBAN::setBitFeature(bitFeature feature);
void VBAN::setExtraBitFeature(extraBitFeature extraFeature);
void VBAN::setDistant(uint8_t *distantIp, uint8_t distantIpLen, uint16_t distantPort);
void VBAN::setDeviceName(uint8_t *name, uint8_t nameLen);
void VBAN::setManufacturerName(uint8_t *name, uint8_t nameLen);
void VBAN::setApplicationName(uint8_t *name, uint8_t nameLen);
void VBAN::setHostName(uint8_t *name, uint8_t nameLen);
void VBAN::setUserName(uint8_t *name, uint8_t nameLen);
void VBAN::setUserComment(uint8_t *name, uint8_t nameLen);