#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <sys/socket.h>
#include <unordered_map>

#include "Channel.h"
#include "Dispatcher/Dispatcher.h"
#include "Logger.h"
#include "TCPConnection.h"
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
public:
    ChannelELEM(Channel* channel, dispatcherOPT opt):m_opt(opt), m_channel(channel){};
public:
    dispatcherOPT m_opt;
    Channel* m_channel;
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
    void addTask(Channel* channel, dispatcherOPT opt);


private:
    void processTask();
private:
    int m_sockPair[2];

    std::atomic<bool> m_isQuit;
    DispatcherType m_dispatcherType;

    std::mutex m_mtx;
    Dispatcher* m_dispatcher;
    std::unordered_map<int, Channel*> m_channelMap;
    std::queue<ChannelELEM> taskQueue;

};
#endif