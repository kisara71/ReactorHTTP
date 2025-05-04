#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "EventLoop.h"
#include "Dispatcher/EpollDispatcher.h"


EventLoop::EventLoop(DispatcherType type):
m_isQuit(false), 
m_dispatcherType(type)
{
    switch (m_dispatcherType)
    {
        case DispatcherType::EPOLL:
            m_dispatcher = new EpollDispatcher(m_channelMap);
            break;
        case DispatcherType::POLL:
            //m_dispatcher = new PollDispatcher;
            break;
        default:
            //m_dispatcher = new SelectDispatcher;
            break;
    }
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockPair);
    if(-1 == ret)
    {
        error("make socket pair failed: %s", strerror(errno));
    }

    Channel* sock2 = new Channel(m_sockPair[0], READEVENT, nullptr,  
        [this](){
        char buf[10];
        recv(this->m_sockPair[0], buf, sizeof(buf), 0);
    });
    m_dispatcher->add(sock2);
    debug("create eventloop");
}
void EventLoop::awakeDispatcher()
{
    debug("awake dispatcher");
    char buf[10] = "wake up";
    send(m_sockPair[1],buf, sizeof(buf), 0);
}
void EventLoop::processTask()
{
    debug("process task");
    {
        std::unique_lock<std::mutex> mtx(m_mtx);
        while(!taskQueue.empty())
        {
            ChannelELEM& elm = taskQueue.front();
            switch(elm.m_opt)
            {
                case dispatcherOPT::ADD:
                    m_dispatcher->add(elm.m_channel);
                    break;
                case dispatcherOPT::MODIFY:
                    m_dispatcher->modify(elm.m_channel);
                    break;
                case dispatcherOPT::REMOVE:
                    m_dispatcher->remove(elm.m_channel);
                    break;
                case dispatcherOPT::CLEAR:
                    m_dispatcher->clear();
                    break;
                default:
                    warn("unsetted ")
                    break;
            }
            taskQueue.pop();
        }
    }
    
}

void EventLoop::run()
{
    debug("ev start run");
    while(!m_isQuit)
    {
        if(!taskQueue.empty())
        {
            processTask();  
        }
        m_dispatcher->dispatch(1);
    }
}
void EventLoop::stop()
{
    m_isQuit.store(true);
    awakeDispatcher();
}
EventLoop::~EventLoop()
{
    debug("destroy a event loop");
    close(m_sockPair[0]);
    close(m_sockPair[1]);
    delete m_dispatcher;

    for(auto& [key, channel]:m_channelMap)
    {
        if(channel != nullptr)
        {
            if(channel->m_deleteCallBack)
            {
                channel->m_deleteCallBack();
            }
        }
    }
    m_channelMap.clear();
}

void EventLoop::addTask(Channel* channel, dispatcherOPT opt)
{
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        taskQueue.emplace(ChannelELEM(channel, opt));
    }
}