#ifndef EPOLL_DISPATCHER_H
#define EPOLL_DISPATCHER_H



#include <sys/epoll.h>
#include "../Dispatcher.h"
#include "../../Channel/Channel.h"
class EpollDispatcher : public Dispatcher
{
public:
    EpollDispatcher(std::unordered_map<int, Channel>& map);
    ~EpollDispatcher();
public:
    bool epollctl(Channel& channel, int opt);
    bool add(Channel& channel) override;
    bool remove(Channel& channel) override;
    bool modify(Channel& channel) override;
    void dispatch(int timeout) override;
    bool clear() override;
private:
    int m_epfd;
    epoll_event* m_events;
};

#endif