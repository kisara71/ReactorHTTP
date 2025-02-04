#include "Server.h"
#include <iostream>
#include "Logger/Logger.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>


bool Server::initListenFd(uint16_t port){
    //监听fd
    m_lfd = socket(AF_INET, SOCK_STREAM , 0);
    if(m_lfd==-1){
        error("create listen fd failed: %s", strerror(errno));
        return false;
    }

    //设置端口复用
    int opt = 1;
    int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(ret==-1){
        error("set reuse addr failed: %s", strerror(errno));
        return false;
    }

    //绑定监听端口
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ret = bind(m_lfd, (struct sockaddr*)& addr, sizeof(addr));
    if(ret==-1){
        error("bind listen fd failed: %s", strerror(errno));
        return false;
    }

    //监听
    ret = listen(m_lfd, 128);
    if(ret==-1){
        error(" listen failed: %s", strerror(errno));
        return false;
    }
    info("start listen on %d", port);
    return true;
}   

void Server::epollRun(){
    //初始化 epfd
    m_epfd = epoll_create(1);
    if(m_epfd==-1){
        fatal("run epoll failed: %s", strerror(errno));
        exit(-1);
    }

    int ret = 0;
    //将监听fd挂到epoll树上
    struct epoll_event ev;
    ev.data.fd = m_lfd;
    ev.events = EPOLLIN;
    ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_lfd, &ev);
    if(ret==-1){
        fatal("add lfd to epoll failed: %s", strerror(errno));
        exit(-1);
    }
    info("run epoll");

    // 定义events接收数组
    struct epoll_event evs[1024];
    constexpr int size = sizeof(evs)/sizeof(evs[0]);
    while(true){
        if(m_stop){
            return;
        }
        int num = epoll_wait(m_epfd, evs, size, 200);
        for(int i=0;i<num;++i){
            int curfd = evs[i].data.fd;
            if(curfd == m_lfd){
                acceptClient();
            }else{

            }
        }


    }
}

bool Server::acceptClient(){
    // 与客户端建立连接
    int client = accept(m_epfd, nullptr, nullptr);
    if(-1==client){
        error("invalid client: %s", strerror(errno));
        return false;
    }
    // 设置client非阻塞
    int flag = fcntl(client, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(client, F_SETFL, flag);

    // 客户端event
    struct epoll_event ev;
    ev.data.fd = client;
    ev.events = EPOLLIN|EPOLLET;

    //添加客户到epoll树
    int ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, client, &ev);
    if(-1 == ret){
        error("add client to epoll failed: %s", strerror(errno));
        return false;
    }
    info("accepted a client");
    return true;
}