#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <unordered_map>

#include "../Channel/Channel.h"
#include "../Dispatcher/Dispatcher.h"

enum class DispatcherType
{
    EPOLL,
    POLL,
    SELECT
};

enum class dispatcherOPT
{
    ADD,
    MODIFY,
    REMOVE,
    CLEAR
};

struct ChannelELEM{
    dispatcherOPT opt;
    Channel& channel;
};

class EventLoop
{
public:
    EventLoop(DispatcherType type = DispatcherType::SELECT);
    ~EventLoop();
private:

    std::mutex m_mtx;
    Dispatcher* m_dispatcher;
    std::atomic<bool> m_isQuit;
    std::thread::id m_threadID;
    std::unordered_map<int, Channel&> channelMap;
    std::queue<ChannelELEM> taskQueue;
};
#endif