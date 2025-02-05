#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/epoll.h>

class Server{
public:
    Server();
    void initServer(uint16_t port);

private:
    bool initListenFd(uint16_t port);
    bool acceptClient();
    bool recvHTTPRequest(int curfd);
    void epollRun();
private:
    int m_lfd;
    int m_epfd;
    bool m_stop;

};


#endif