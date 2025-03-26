#include "ThreadPool.h"
ThreadPool::ThreadPool(int threadCount)
{
   { 
    std::unique_lock<std::mutex> lock(mtx);
    for(int i = 1;i<=threadCount;++i)
    {
        std::thread t(&ThreadPool::worker, this);
        std::thread::id tid = t.get_id();
        EventLoop* ev = new EventLoop(DispatcherType::EPOLL);
        m_workers[tid] = std::make_pair(std::move(t), ev);
    }
    }
}
ThreadPool::~ThreadPool()
{
    for(auto it = m_workers.begin();it!=m_workers.end();++it)
    {
        it->second.second->stop();
        if(it->second.first.joinable())
        {
            it->second.first.join();
            
        }
        delete it->second.second;
    }
}

  
void ThreadPool::worker()
{
    EventLoop* ev;
    {
        std::unique_lock<std::mutex> lock (mtx);
        ev = m_workers[std::this_thread::get_id()].second;
    }
    ev->run();
}