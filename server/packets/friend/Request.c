#include <protocol/CRPPackets.h>
#include "run/user.h"
#include <asm-generic/errno.h>
#include <stdlib.h>

int ProcessPacketFriendRequest(POnlineUser user, uint32_t session, CRPPacketFriendRequest *packet)
{
    if (user->state == OUS_ONLINE)
    {
        pthread_rwlock_rdlock(user->info->friendsLock);
        size_t length = UserFriendsSize(user->info->friends);
        void *data = malloc(length);
        UserFriendsEncode(user->info->friends, data);
        pthread_rwlock_unlock(user->info->friendsLock);
        CRPSend(user->crp, CRP_PACKET_FRIEND_DATA, 0, data, (CRP_LENGTH_TYPE) length);

        free(data);
    }
    else
    {
        CRPFailureSend(user->crp, session, EACCES, "状态错误");
    }
    return 1;
}