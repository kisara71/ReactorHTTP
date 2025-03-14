#include "EventLoop.h"
#include "../Dispatcher/EpollDispatcher/EpollDispatcher.h"
#include "../Dispatcher/PollDispatcher/PollDispatcher.h"
#include "../Dispatcher/SelectDispatcher/SelectDispatcher.h"


EventLoop::EventLoop(DispatcherType type):
m_isQuit(false), 
m_threadID(std::this_thread::get_id())
{
    switch (type)
    {
        case DispatcherType::EPOLL:
            m_dispatcher = new EpollDispatcher;
            break;
        case DispatcherType::POLL:
            m_dispatcher = new PollDispatcher;
            break;
        default:
            m_dispatcher = new SelectDispatcher;
            break;
    }
}

EventLoop::~EventLoop()
{
    delete m_dispatcher;
}