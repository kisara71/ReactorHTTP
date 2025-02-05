#ifndef SERVER_H
#define SERVER_H
class HTTPRequest;
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "../HTTPRequest/HTTPRequest.h"
class Server{
public:
    Server();
    void initServer(uint16_t port);

private:
    bool initListenFd(uint16_t port);
    bool acceptClient();
    void epollRun();
private:
    int m_lfd;
    int m_epfd;
    bool m_stop;

    HTTPRequest http;

};


#endif