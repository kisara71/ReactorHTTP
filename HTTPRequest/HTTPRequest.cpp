#include "HTTPRequest.h"
#include <string>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "../Logger/Logger.h"

bool HTTPRequest::recvHTTPRequest(int curfd)
{
    std::string requestMSG;
    requestMSG.reserve(4096);
    char temp[1024];

    int len = 0;
    while (true)
    {
        len = recv(curfd, temp, sizeof(temp), 0);
        if (len > 0)
        {
            // 正常接收数据
            requestMSG.append(temp, len);
        }
        else if (0 == len)
        {
            // client 断开连接
            epoll_ctl(m_epfd, EPOLL_CTL_DEL, curfd, nullptr);
            close(curfd);
            return false;
        }
        else if (-1 == len)
        {
            if (errno == EAGAIN)
            {
                // 数据接收完毕
                break;
            }
            else if (errno == EINTR)
            {
                // 意外中断
                continue;
            }
            else
            {
                error("recv http request failed: %s", strerror(errno));
                return false;
            }
        }
    }

    return true;
}

void HTTPRequest::parseHTTPRequest(const char* request){
    httpINFO temp;
    sscanf(request, "%[^ ] %[^ ]", temp.method, temp.requestLine);
}
void HTTPRequest::init(int epfd){
    m_epfd = epfd;

    info("http inited");
}

HTTPRequest::HTTPRequest(){

}

HTTPRequest::~HTTPRequest(){

}
