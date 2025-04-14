#include "ThreadPool.h"
ThreadPool::ThreadPool(int threadCount, DispatcherType type):
m_threadCount(threadCount),
m_index(0)
{
   { 
    std::unique_lock<std::mutex> lock(mtx);
    for(int i = 1;i<=m_threadCount;++i)
    {
        std::thread t(&ThreadPool::worker, this, type);
        m_workers.emplace_back(std::move(t));
    }
    }
}
ThreadPool::~ThreadPool()
{
   for(auto& ev : m_evLoops)
   {
        ev->stop();
   }
   for(auto& t : m_workers)
   {
    if(t.joinable())
    {
        t.join();
    }
   }
   for(auto it = m_evLoops.begin();it!=m_evLoops.end();++it)
   {
        delete *it;
   }
}

EventLoop* ThreadPool::getEvLoop()
{
    if(0 < m_threadCount)
    {
        return m_evLoops[(m_index++)%m_threadCount];
    }else{
        return nullptr;
    }
}
  
void ThreadPool::worker(DispatcherType type)
{
    EventLoop* ev = new EventLoop(type);
    {
        std::unique_lock<std::mutex> lock (mtx);
        m_evLoops.emplace_back(ev);
    }
    ev->run();
}