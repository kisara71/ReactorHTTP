#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include "Dispatcher/EpollDispatcher.h"
#define DEFAULT_COUNTS 1024

EpollDispatcher::EpollDispatcher(std::unordered_map<int, Channel*>& map)
:Dispatcher(map)
{
    m_events = new epoll_event[DEFAULT_COUNTS];

    m_epfd = epoll_create(1);
    if(-1 == m_epfd)
    {
        error("init epoll dispatcher failed: %s", strerror(errno));
    }
}

EpollDispatcher::~EpollDispatcher()
{
    debug("destroy a epoll dispatcher");
    if(m_events != nullptr)
    {
        delete [] m_events;
    }
    close(m_epfd);
}
bool EpollDispatcher::epollctl(Channel* channel, int opt)
{
    
    epoll_event ev;
    ev.events = 0;
    ev.data.fd = channel->m_fd;
    if(channel->m_events & WRITEEVENT)
    {
        ev.events |= EPOLLOUT;
    }
    if(channel->m_events & READEVENT)
    {
        ev.events |= EPOLLIN;
    }

    int ret = epoll_ctl(m_epfd, opt, channel->m_fd, &ev);
    return (-1 != ret);
}

bool EpollDispatcher::add(Channel* channel)
{
    bool ret = epollctl(channel, EPOLL_CTL_ADD);
    if(!ret)
    {
        error("epoll dispatcher add failed :%s", strerror(errno));
        return false;
    }
    m_channelMap[channel->m_fd] = channel;
    debug("epoll dispatcher add success");
    return true;
}

bool EpollDispatcher::remove(Channel* channel)
{
    bool ret = epollctl(channel, EPOLL_CTL_DEL);
    if(!ret)
    {
        error("epoll dispatcher delete failed :%s", strerror(errno));
        return false;
    }
    int fd = channel->m_fd;
    m_channelMap.erase(fd);
    channel->m_deleteCallBack();
    debug("epoll dispatcher delete success");
    return true;
}

bool EpollDispatcher::modify(Channel* channel)
{
    bool ret = epollctl(channel, EPOLL_CTL_MOD);
    if(!ret)
    {
        error("epoll dispatcher modify failed :%s", strerror(errno));
        return false;
    }
    debug("epoll dispatcher modify success");
    return true;
}

void EpollDispatcher::dispatch(int timeout)
{
    int count = epoll_wait(m_epfd, m_events, DEFAULT_COUNTS, timeout * 1000);
    for(int i = 0; i < count; ++i)
    {
        int events = m_events[i].events;
        int curfd = m_events[i].data.fd;
        Channel* curChannel = m_channelMap[curfd];
        if(events & EPOLLERR || events & EPOLLHUP)
        {
           remove(curChannel);
           continue;
        }
        if(events & EPOLLIN)
        {
            if(curChannel->m_readCallBack)
            {
                debug("read call back ");
                curChannel->m_readCallBack();
            }else{
                curChannel->defaultReadCallBack();
                debug("default read call back");
            }
        }
        if(events & EPOLLOUT)
        {
            if(curChannel->m_writeCallBack)
            {
                curChannel->m_writeCallBack();
                debug("write call back");
            }else{
                warn("unbind writeCallBack");
            }
        }
    }
}

bool EpollDispatcher::clear()
{
    if(m_events != nullptr)
    {
        delete [] m_events;
        return true;
    }

   
    return false;
}