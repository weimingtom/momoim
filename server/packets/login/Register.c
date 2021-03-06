#include <protocol/CRPPackets.h>
#include "run/user.h"
#include <string.h>
#include <asm-generic/errno-base.h>
#include <datafile/message.h>
#include "datafile/auth.h"
#include "datafile/user.h"
#include "datafile/friend.h"

int ProcessPacketLoginRegister(POnlineUser user, uint32_t session, CRPPacketLoginRegister *packet)
{
    if (user->state == OUS_PENDING_LOGIN)
    {
        uint32_t uid;
        uid = AuthRegister(packet->username, packet->password);
        if (uid > 0)
        {
            UserCreateDirectory(uid);
            UserFriendsCreate(uid);
            UserInfo *info = UserInfoGet(uid);
            bzero(info->nickName, sizeof(info->nickName));
            memcpy(info->nickName, packet->nickname,
                    packet->nicknameLength > sizeof(info->nickName) ? sizeof(info->nickName) : packet->nicknameLength);
            UserInfoSave(uid, info);
            UserInfoDrop(info);
            UserMessageFileCreate(uid);
            CRPOKSend(user->crp, session);
        }
        else
        {
            CRPFailureSend(user->crp, session, EEXIST, "用户名已存在");
        }
    }
    else
    {
        CRPFailureSend(user->crp, session, EACCES, "状态错误");
    }
    return 1;
}