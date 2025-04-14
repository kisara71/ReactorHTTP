#include "Channel.h"
#include "../TCPConnection/TCPConnection.h"
#include <unistd.h>
#include <iostream>

Channel::Channel(int fd, uint8_t events, TCPConnection* conn, handleFunc readCallBack, handleFunc writeCallBack, handleFunc deleteCallBack):
m_readCallBack(readCallBack),
m_writeCallBack(writeCallBack),
m_deleteCallBack(deleteCallBack),
m_fd(fd), 
m_events(events),
m_tcpCon(conn)
{

}

Channel::~Channel(){

    close(m_fd);
}
void Channel::setWriteEvent(bool flag){
    if(flag){
       m_events |= WRITEEVENT;
    }else{
        m_events &= (~WRITEEVENT);
    }
}
void Channel::setReadEvent(bool flag){
    if(flag){
       m_events |= READEVENT;
    }else{
        m_events &= (~READEVENT);
    }
}


void Channel::defaultReadCallBack()
{
    m_tcpCon->handleRead();
}
