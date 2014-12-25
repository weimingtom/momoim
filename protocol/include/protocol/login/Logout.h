#pragma once
/**
* 登出当前回话，返回PENDING_LOGIN状态
* 不应在PENDING_HELLO状态发送此包
*/
typedef struct
{
} CRPPacketLoginLogout;

/**
* 登出包转换
*/
CRPPacketLoginLogout *CRPLoginLogoutCast(CRPBaseHeader *base);

/**
* 发送登出包
*/
int CRPLoginLogoutSend(int sockfd);