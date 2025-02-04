#include "Server.h"
#include <arpa/inet.h>
#include <iostream>
bool Server::initListenFd(uint16_t port){
    //监听fd
    m_lfd = socket(AF_INET, SOCK_STREAM , 0);
    if(m_lfd==-1){
        std::perror("lfd create failed: ");
        return false;
    }

    //设置端口复用
    int opt = 1;
    int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(ret==-1){
        std::perror("setsocketopt failed: ");
        return false;
    }

    //绑定监听端口
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ret = bind(m_lfd, (struct sockaddr*)& addr, sizeof(addr));
    if(ret==-1){
        std::perror("bind failed: ");
        return false;
    }

    //监听
    ret = listen(m_lfd, 128);
    if(ret==-1){
        std::perror("listen failed: ");
        return false;
    }
    return true;
}   

void Server::epollRun(){
    //初始化 epfd
    m_epfd = epoll_create(1);
    if(m_epfd==-1){
        std::perror("create epoll fd failed");
        exit(-1);
    }

    int ret = 0;
    //将监听fd挂到epoll树上
    struct epoll_event ev;
    ev.data.fd = m_lfd;
    ev.events = EPOLLIN;
    ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_lfd, &ev);
    if(ret==-1){
        std::perror("add lfd to epoll failed");
        exit(-1);
    }
}