#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <sys/socket.h>
#include <unordered_map>

#include "../Channel/Channel.h"
#include "../Dispatcher/Dispatcher.h"
#include "../Logger/Logger.h"

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

public:
    void run();
    void stop();
    void awakeDispatcher();
    int m_sockPair[2];


private:
    void processTask();
private:


    std::atomic<bool> m_isQuit;
    DispatcherType m_dispatcherType;

    std::mutex m_mtx;
    Dispatcher* m_dispatcher;
    std::unordered_map<int, Channel> channelMap;
    std::queue<ChannelELEM> taskQueue;
};
#endif