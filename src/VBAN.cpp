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
        if(subscribedStream[i] != _header_ptr->streamName[i]){
            return STREAM_NOT_SUBSCRIBED;
        }
    }

    switch (_header_ptr->subProtocol){
        case PROT_AUDIO:
            return handleAudio(packet, packetSize);

        case PROT_SERIAL:
            return handleSerial(packet, packetSize);

        case PROT_TXT:
            return handleText(packet, packetSize);
        
        case PROT_USER:
            return handleUser(packet, packetSize);

        case PROT_FRAME:
            return handleFrame(packet, packetSize);

        case PROT_SERVICE:
            return handleService(packet, packetSize);
        
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

    int16_t _buffer[numChannels * numSamples] = {0};
    decodePCM()

}

VBAN::returnCodes VBAN::decodePCM(uint8_t *data, uint16_t dataLen, audioDataTypes dataType, int16_t *result, uint16_t resultLen) {
    
    if (dataLen > resultLen) {
        return BUFFER_TOO_SMALL;
    }
    
    for (uint16_t i = 0; i < dataLen; i++){
        switch (dataType) {
            case ADT_BYTE8:
                result[i] = static_cast<int16_t> ((data[i] - 128) * 256);
                break;

            case ADT_INT16:
                result[i] = data[i];
                break;

            case ADT_INT24:
                result[i] = static_cast<int16_t> (
                    (data[i * 3] << 16 | 
                    data[i * 3 + 1] << 8 |
                    data[i * 3 + 2])/256
                );
                break;

            case ADT_INT32:
                result[i] = static_cast<int16_t>(
                    (data[i*4] << 24 |
                    data[i*4 + 1] << 16 |
                    data[i*4 + 2] << 8 |
                    data[i*4 + 3])/65538
                );
                break;

            case ADT_FLOAT32:
                result[i] = static_cast<float>(data[i*4]<<24|data[i*4+1]<<16|data[i*4+2]<<8|data[i*4+3])*32767.0;
                break;

            case ADT_FLOAT64:
                break;

            case ADT_12BITS:
                break;

            case ADT_10BITS:
                break;

            default:
                return UNSUPPORTED_CODEC;
        }
    }


    return SUCCESS;
}

VBAN::returnCodes VBAN::sendPing0(uint8_t *targetIp, uint8_t targetIpLen, uint16_t targetPort){

    union {
        servicePing format;
        uint8_t raw[sizeof(servicePing)];
    }_packet;

    _packet.format.bitType = static_cast <uint32_t> (sysConf.deviceType);

    //not sure of multiple features at the same time are possible
    _packet.format.bitFeature = static_cast <uint32_t> (sysConf.feature);
    _packet.format.bitFeatureEx = static_cast <uint32_t> (sysConf.extraFeature);

    _packet.format.preferedRate = sysConf.prefRate;
    _packet.format.maxRate = sysConf.maxRate;
    _packet.format.minRate = sysConf.minRate;

    _packet.format.colorRGB = (sysConf.color.red << 24) | (sysConf.color.green << 16) | (sysConf.color.blue << 8);

    for(uint8_t i = 0; i<4; i++){
        _packet.format.nVersion[i] = version[i];
    }

    copyPosition(sysConf.gpsPosition, &_packet.format.GPS_Postion);
    copyPosition(sysConf.userPosition, &_packet.format.USER_Position);

    for (uint8_t i = 0; i < sizeof(_packet.format.langCodeAscii); i++) {
        uint8_t c = static_cast <uint8_t>(sysConf.langCode >> (i * 8));
        _packet.format.langCodeAscii[i] = replaceNoneAsciiChar(c);
    }

    //@TODO: unclear what the protocol expects here
    for (uint8_t i = 0; i < maxDistantIpLen; i++) {
        _packet.format.distantIPAscii[i] = 0;
    }
    _packet.format.distantPort = 0;

    for(uint8_t i = 0; i<maxDeviceNameLen; i++){
        _packet.format.deviceNameAscii[i] = sysConf.deviceName[i];
    }

    for(uint8_t i = 0; i<maxManufacturerNameLen; i++){
        _packet.format.manufacturerNameAscii[i] = sysConf.manufacturerName[i];
    }

    for(uint8_t i = 0; i<maxApplicationNameLen; i++){
        _packet.format.applicationNameAscii[i] = sysConf.applicationName[i];
    }

    for(uint8_t i = 0; i<maxHostNameLen; i++){
        _packet.format.hostNameAscii[i] = sysConf.hostName[i];
    }

    for(uint8_t i = 0; i<maxUserNameLen; i++){
        _packet.format.userNameUtf8[i] = sysConf.userName[i];
    }

    for(uint8_t i = 0; i<maxUserCommentLen; i++){
        _packet.format.userCommentUtf8[i] = sysConf.userComment[i];
    }

    return callback_sendUDP(_packet.raw, sizeof(_packet.raw),targetIp, targetIpLen, targetPort);
}

void VBAN::copyPosition(GPSPosition source, GPSPosition *target){
    target->latitudeDegree = source.latitudeDegree;
    target->latitudeMinute = source.latitudeMinute;
    target->latitudeSecond = source.latitudeSecond;
    target->latitudeScent  = source.latitudeScent;

    target->longitudeDegree = source.longitudeDegree;
    target->longitudeMinute = source.longitudeMinute;
    target->longitudeSecond = source.longitudeSecond;
    target->longitudeScent  = source.longitudeScent;
}

void VBAN::setGPSPosition(GPSPosition postion){
    copyPosition(postion, &sysConf.gpsPosition);
}

void VBAN::setUSerPostion(GPSPosition position){
    copyPosition(position, &sysConf.userPosition);
}

void VBAN::setLanguage(langCodes langCode){
    sysConf.langCode = langCode;
}

void VBAN::setColor(uint8_t red, uint8_t green, uint8_t blue){
    sysConf.color.red = red;
    sysConf.color.green = green;
    sysConf.color.blue = blue;
}

VBAN::returnCodes VBAN::setRate(uint32_t minRate, uint32_t prefRate, uint32_t maxRate){

    if(!minRate < maxRate){
        return INVALID_PARAMETERS;
    }

    if(!(minRate < prefRate && prefRate < maxRate)){
        return INVALID_PARAMETERS;
    }

    sysConf.minRate = minRate;
    sysConf.prefRate = prefRate;
    sysConf.maxRate = maxRate;

    return SUCCESS;
}

void VBAN::setDeviceType(bitType type){
    sysConf.deviceType = type;
}

void VBAN::setBitFeature(bitFeature feature){
    sysConf.feature = feature;
}

void VBAN::setExtraBitFeature(extraBitFeature extraFeature){
    sysConf.extraFeature = extraFeature;
}

VBAN::returnCodes VBAN::setDistant(uint8_t *distantIp, uint8_t distantIpLen, uint16_t distantPort){
    if (distantIpLen != 0){
        return NOT_IPV4;
    }

    return SUCCESS;
}

void VBAN::setDeviceName(uint8_t *name, uint8_t nameLen){

    if (nameLen < 1) {
        return;
    }

    if (nameLen > maxDeviceNameLen){
        nameLen = maxDeviceNameLen;
    }

    for(uint8_t i = 0; i<nameLen; i++) {
        sysConf.deviceName[i] = replaceNoneAsciiChar(name[i]);
    }
}

uint8_t VBAN::replaceNoneAsciiChar(uint8_t c){
    if (c > 127){
        return '?';
    }
    return c;
}


void VBAN::setManufacturerName(uint8_t *name, uint8_t nameLen) {
    
    if (nameLen < 1) {
        return;
    }

    if (nameLen > maxManufacturerNameLen){
        nameLen = maxManufacturerNameLen;
    }

    for(uint8_t i = 0; i<nameLen; i++) {
        sysConf.manufacturerName[i] = replaceNoneAsciiChar(name[i]);
    }
}

void VBAN::setApplicationName(uint8_t *name, uint8_t nameLen) {
    if (nameLen < 1) {
        return;
    }

    if (nameLen > maxApplicationNameLen) {
        nameLen = maxApplicationNameLen;
    }

    for (uint8_t i = 0; i < nameLen; i++) {
        sysConf.applicationName[i] = replaceNoneAsciiChar(name[i]);
    }
}

void VBAN::setHostName(uint8_t *name, uint8_t nameLen) {
    if (nameLen < 1) {
        return;
    }

    if (nameLen > maxHostNameLen) {
        nameLen = maxHostNameLen;
    }

    for (uint8_t i = 0; i<nameLen; i++){
        sysConf.hostName[i] = replaceNoneAsciiChar(name[i]);
    }
}
void VBAN::setUserName(uint8_t *name, uint8_t nameLen) {
    if (nameLen < 1) {
        return;
    }

    if (nameLen > maxUserNameLen) {
        nameLen = maxUserNameLen;
    }

    for (uint8_t i = 0; i < nameLen; i++) {
        sysConf.userName[i] = name[i];
    }
}

void VBAN::setUserComment(uint8_t *comment, uint8_t commentLen) {
    if (commentLen < 1) {
        return;
    }

    if (commentLen > maxUserCommentLen) {
        commentLen = maxUserCommentLen;
    }

    for (uint8_t i = 0; i < commentLen; i++){
        sysConf.userComment[i] = comment[i];
    }
}

VBAN::VBAN(uint8_t *ManufacturerName, uint8_t ManufacturerNameLen){
    setManufacturerName(ManufacturerName, ManufacturerNameLen);
}