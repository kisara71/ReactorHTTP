#ifndef THREADPOOL_H
#define THREADPOOL_H


#include <thread>
#include <mutex>
#include <vector>
#include <memory>

#include "../EventLoop/EventLoop.h"
class ThreadPool
{

public:
    ThreadPool(int threadCount, DispatcherType type);
    ~ThreadPool();

public:
    EventLoop* getEvLoop();

private:
    void worker(DispatcherType type);

private:
    int m_threadCount;
    int m_index;
    std::mutex mtx;
    std::vector<EventLoop*> m_evLoops;
    std::vector<std::thread> m_workers;
};


#endif