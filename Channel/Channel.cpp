#include "Channel.h"
#include <unistd.h>
#include <iostream>
Channel::Channel(uint8_t events):
m_fd(-1), 
m_events(events),
USELESS(false)
{

}
void Channel::setUseless(bool flag)
{
    USELESS = flag;
}
Channel::~Channel(){
    if(USELESS)
    {
        close(m_fd);
    }
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

void Channel::bindReadCallBack(std::function<bool(void)> func)
{
    readCallBack = std::move(func);
    return;
}

void Channel::bindWriteCallBack(std::function<bool(void)> func)
{
    writeCallBack = std::move(func);
    return;
}

