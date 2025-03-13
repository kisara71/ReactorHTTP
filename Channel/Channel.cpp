#include "Channel.h"
#include <unistd.h>
Channel::Channel():m_fd(-1), m_events(READEVENT) {

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


