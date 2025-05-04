#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "ThreadPool.h"
#include "EventLoop.h"

class TCPServer
{
public:
    TCPServer(int threadCount, unsigned short port = 55655, DispatcherType type = DispatcherType::SELECT);
    ~TCPServer();
public:
    void run();
    void stop();


private:
    int m_listenFD;
    unsigned short m_port;

    EventLoop* m_evLoop;
    ThreadPool* m_tPool;


};

#endif