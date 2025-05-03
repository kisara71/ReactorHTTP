#include <string.h>
#include <arpa/inet.h>

#include "TCPServer.h"
#include "../Logger/Logger.h"


TCPServer::TCPServer(int threadCount, unsigned short port, DispatcherType type):
m_port(port)
{
    m_listenFD = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_listenFD)
    {
        error("create tcp server failed:%s", strerror(errno));
    }

    int opt = 1;
    int ret = setsockopt(m_listenFD, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if(-1 == ret)
    {
        error("set tcp server reuseport failed: %s", strerror(errno));
    }

    sockaddr_in addrin;
    addrin.sin_addr.s_addr = INADDR_ANY;
    addrin.sin_family = AF_INET;
    addrin.sin_port = htons(m_port);

    ret = bind(m_listenFD, (sockaddr*) &addrin, sizeof(addrin));
    if(-1 == ret)
    {
        error("bind listenFD failed: %s", strerror(errno));
    }

    ret = listen(m_listenFD, 128);
    if(-1 == ret)
    {
        error("start listen failed: %s", strerror(errno));
    }

    m_evLoop = new EventLoop(type);
    m_tPool = new ThreadPool(threadCount, type);

    Channel* channel = new Channel(m_listenFD, READEVENT, nullptr, 
        [this](){ 

        int fd = accept(m_listenFD, nullptr, nullptr);
        debug("accept a client")

        EventLoop* ev = m_tPool->getEvLoop();
        if(!ev)
        {
            ev = m_evLoop;
        }
        TCPConnection* conn = new TCPConnection(fd, READEVENT);
        conn->bindDisconnect([ev, conn](){
            ev->addTask(conn->m_channel, dispatcherOPT::REMOVE);
            ev->awakeDispatcher();
        });
        conn->bindModifyWrite([ev, conn](){
            ev->addTask(conn->m_channel, dispatcherOPT::MODIFY);
            ev->awakeDispatcher();
        });
        ev->addTask(conn->m_channel, dispatcherOPT::ADD);

        ev->awakeDispatcher();

        return true;
    });

    m_evLoop->addTask(channel, dispatcherOPT::ADD);
}

void TCPServer::stop()
{
    m_evLoop->stop();
}

TCPServer::~TCPServer()
{
    delete m_evLoop;
    delete m_tPool;
}

void TCPServer::run()
{
    m_evLoop->run();
}
