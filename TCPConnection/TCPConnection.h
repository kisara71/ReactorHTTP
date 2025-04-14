#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H


#include "../Buffer/Buffer.h"
#include "../Channel/Channel.h"

class TCPConnection
{
public:
    TCPConnection(int fd,Events events);
    ~TCPConnection();
public:
    void handleRead();
    void handleWrite();

    void bindDisconnect(handleFunc&& func);
    
    handleFunc m_disconnect;

public:
    Channel* m_channel;

    int m_curReadByte;
    Buffer* m_readBuf;
    Buffer* m_writeBuf;

};


#endif