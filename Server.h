#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/epoll.h>

class Server{
public:
    void initServer();

private:
    bool initListenFd(uint16_t port);
    bool acceptClient();
    void epollRun();
private:
    int m_lfd;
    int m_epfd;
    bool m_stop;

};


#endif