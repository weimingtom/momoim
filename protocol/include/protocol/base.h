#pragma once

#include <sys/types.h>
#include <stdint.h>
typedef __uint16_t CRP_LENGTH_TYPE;

typedef struct
{
    unsigned int magicCode /* 0x464F5573 */;
    CRP_LENGTH_TYPE totalLength; //协议包总长度，也用于支持不同协议版本
    CRP_LENGTH_TYPE dataLength;  //数据块总长度，用于接下来接收数据块
    uint16_t packetID;           //包ID，用于区分不同数据包
    char data[0];                //包数据部分
} CRPBaseHeader;

//用于打包并发送CRP包
ssize_t CRPSend(uint16_t packetID, void *data, size_t length, int fd);

//用于接收一个CRP包
CRPBaseHeader *CRPRecv(int fd);