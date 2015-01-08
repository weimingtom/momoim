#pragma once

#include "../base.h"
#include <stdint.h>
#include "imcommon/message.h"

CRP_STRUCTURE
{
    uint16_t seq;
    char pad[1024 - sizeof(uint16_t)];
} CRPPacketMessageDiscover;

__attribute_malloc__
CRPPacketMessageDiscover *CRPMessageDiscoverCast(CRPBaseHeader *base);

int CRPMessageDiscoverSend(CRPContext context, uint32_t sessionID, uint32_t uid, char key[32]);