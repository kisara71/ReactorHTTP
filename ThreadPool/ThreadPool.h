#ifndef THREADPOOL_H
#define THREADPOOL_H


#include <thread>
#include <mutex>
#include <unordered_map>

#include "../EventLoop/EventLoop.h"
class ThreadPool
{

public:
    ThreadPool(int threadCount);
    ~ThreadPool();

private:
    void worker();

private:
    std::mutex mtx;
    std::unordered_map<std::thread::id, std::pair<std::thread, EventLoop*>> m_workers;
};


#endif