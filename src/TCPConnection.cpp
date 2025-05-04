#include "TCPConnection.h"
#include "Logger.h"
TCPConnection::TCPConnection(int fd, Events events):
m_readBuf(new Buffer(10240)),
m_http(new HTTP(fd)),
m_channel(new Channel(fd, events, this, nullptr, 
    [this]()
    {
        m_http->handleResponse();
        m_channel->setWriteEvent(false);
        this->m_modifyWrite();
    }, 
    [this]()
    {
        delete this;
    }))
{
    
}

TCPConnection::~TCPConnection()
{
    delete m_readBuf;
    delete m_channel;
    delete m_http;

}

void TCPConnection::handleRead()
{
    debug("handle read");
    m_curReadByte = m_readBuf->readFromFD(m_channel->m_fd);
    if(m_curReadByte == 0)
    {
        m_disconnect();
        return;
    }
    m_http->handleRequest(m_readBuf);
    m_readBuf->reset();

    m_channel->setWriteEvent(true);
    m_modifyWrite();

    return;
}

void TCPConnection::bindDisconnect(handleFunc&& func)
{
    m_disconnect = std::move(func);
}

void TCPConnection::bindModifyWrite(handleFunc&& func)
{
    m_modifyWrite = std::move(func);
}