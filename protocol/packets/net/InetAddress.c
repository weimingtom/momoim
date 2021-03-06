#include <protocol/CRPPackets.h>


CRPPacketNETInetAddress *CRPNETInetAddressCast(CRPBaseHeader *base)
{
    return (CRPPacketNETInetAddress *) base->data;
}

int CRPNETInetAddressSend(CRPContext context, uint32_t sessionID, uint32_t uid, uint32_t ipv4, uint16_t port)
{
    CRPPacketNETInetAddress packet = {
            .uid=uid,
            .ipv4=ipv4,
            .port=port
    };
    return CRPSend(context, CRP_PACKET_NET_INET_ADDRESS, sessionID, &packet, sizeof(packet)) != -1;
}