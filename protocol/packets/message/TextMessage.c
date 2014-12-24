#include <protocol/packets.h>
#include <stddef.h>
#include <bits/string3.h>
#include "../../include/protocol/base.h"
#include "../../include/protocol/message/TextMessage.h"

CRPPacketTextMessage *CRPTextMessageCast(CRPBaseHeader *base)
{
    return (CRPPacketTextMessage *) base->data;
}

int CRPTextMessageSend(int sockfd, uint32_t userid, uint32_t sendtime, __uint16_t message_len, char *message)
{
    CRPPacketTextMessage *packet = (CRPPacketTextMessage *)mallac(sizeof(CRPPacketTextMessage)+ message_len );
    packet->message_len = message_len;
    packet->userid = userid;
    packet->sendtime = sendtime;
    memcpy(packet->message, message,message_len);
    ssize_t ret = CRPSend(CRP_PACKET_MESSAGE_TEXT, packet, mallac(sizeof(CRPPacketTextMessage)+ message_len, sockfd));
    free(packet);
    return ret != -1;
}