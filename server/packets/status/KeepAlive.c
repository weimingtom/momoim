#include <protocol/CRPPackets.h>
#include "run/user.h"

int ProcessPacketStatusKeepAlive(POnlineUser user, uint32_t session, CRPPacketKeepAlive *packet)
{
    CRPOKSend(user->crp, session);
    return 1;
}