#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H


#include "../Buffer/Buffer.h"
#include "../Channel/Channel.h"
#include "../HTTP/HTTP.h"
class TCPConnection
{
public:
    TCPConnection(int fd,Events events);
    ~TCPConnection();
public:
    void handleRead();

    void bindDisconnect(handleFunc&& func);
    void bindModifyWrite(handleFunc&& func);
    handleFunc m_disconnect;
    handleFunc m_modifyWrite;

public:
    Buffer* m_readBuf;
    HTTP* m_http;
    Channel* m_channel;
    int m_curReadByte;
};


#endif