#include "TCPConnection.h"
#include "../Logger/Logger.h"
TCPConnection::TCPConnection(int fd, Events events):
m_channel(new Channel(fd, events, this, nullptr, nullptr, [this](){delete this;}))
{
    m_readBuf = new Buffer(10240);
    m_writeBuf = new Buffer(10240);
}

TCPConnection::~TCPConnection()
{
    delete m_readBuf;
    delete m_writeBuf;
    if(m_channel!=nullptr){
        delete m_channel;
    }
}

void TCPConnection::handleRead()
{
    debug("handle read");
    m_curReadByte = m_readBuf->readFromFD(m_channel->m_fd);
    if(m_curReadByte == 0)
    {
        m_disconnect();
    }
    return;
}

void TCPConnection::bindDisconnect(handleFunc&& func)
{
    m_disconnect = std::move(func);
}