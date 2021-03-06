#include <protocol/CRPPackets.h>
#include <asm-generic/errno-base.h>
#include "run/user.h"

int ProcessPacketStatusCancel(POnlineUser user, uint32_t session, CRPPacketCancel *packet)
{
    PUserOperation op = UserOperationGet(user, session);
    if (op)
    {
        UserOperationCancel(user, op);
        CRPOKSend(user->crp, session);
    }
    else
    {
        CRPFailureSend(user->crp, session, ENOENT, "无法找到指定操作");
    }
    return 0;
}