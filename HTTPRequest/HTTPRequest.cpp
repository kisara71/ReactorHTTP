#include "HTTPRequest.h"
#include <string>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "../Logger/Logger.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <dirent.h>
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
    info("get a request: %s", requestMSG.c_str());
    parseHTTPRequest(curfd, requestMSG.c_str());
    return true;
}

bool HTTPRequest::  parseHTTPRequest(int curfd, const char* request){
    httpINFO temp;
    sscanf(request, "%[^ ] %[^ ]", temp.method, temp.requestLine);
    info("request line:\n %s", temp.requestLine);
    if(strcmp(temp.method, "GET") == 0){
        const char* file = nullptr;
        if(strcmp(temp.requestLine, "/") == 0){
            file = "./";
        }else{
            file = temp.requestLine+1;
        }
        struct stat st;
        int ret = stat(file, &st);
        if(-1 == ret){
            // 返回404
            sendHTTPHead(curfd, HTTPRequest::HTTP_NOTFOUND, getFileType("404.html").c_str(), "Not Found");
            sendHTTPFile(curfd, "404.html");
            warn("file not found: %s", file);
            return false;
        }
        if(S_ISDIR(st.st_mode)){
            //发送目录
            sendHTTPHead(curfd, HTTPRequest::HTTP_OK, getFileType("dir.html").c_str(), "OK");
            sendHTTPDir(curfd, file);
        }else{
            //发送文件
            sendHTTPHead(curfd, HTTPRequest::HTTP_OK, getFileType(file).c_str(), "OK", st.st_size);
            sendHTTPFile(curfd, file);
        }
    }
    return true;
}
void HTTPRequest::init(int epfd){
    m_epfd = epfd;
    info("http inited");
}

HTTPRequest::HTTPRequest(){

}

HTTPRequest::~HTTPRequest(){

}
bool HTTPRequest::sendHTTPFile(int curfd, const char* filename){

    int file = open(filename, O_RDONLY);
    if(-1 == file){
        error("open file failed: %s", strerror(errno));
        return false;
    }
    struct stat st;
    stat(filename, &st);
    info("send size is: %d", st.st_size);
    sendfile(curfd, file, nullptr, st.st_size);
    close(file);
    return true;
}

bool HTTPRequest::sendHTTPHead(int curfd, HTTPRequest::HTTPStatus status, const char* type, const char* msg, int size){
    char head[1024];
    sprintf(head, "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", static_cast<int>(status), msg, type, size);
    send(curfd, head, strlen(head), 0);
    return true;
}

std::string HTTPRequest::getFileType(const std::string& filename){
    std::string::size_type idx = filename.rfind('.');
    if(idx == std::string::npos){
        return "text/plain";
    }
    std::string type = filename.substr(idx+1);
    if(type == "html" || type == "htm"){
        return "text/html";
    }else if(type == "jpg" || type == "jpeg"){
        return "image/jpeg";
    }else if(type == "png"){
        return "image/png";
    }else if(type == "gif"){
        return "image/gif";
    }else if(type == "css"){
        return "text/css";
    }else if(type == "js"){
        return "application/x-javascript";
    }else if(type == "ico"){
        return "image/x-icon";
    }else{
        return "text/plain";
    }
}

bool HTTPRequest::sendHTTPDir(int curfd, const char* dirName){
    struct dirent** fileList;
    char toSendBuf[4096] = {0};
    sprintf(toSendBuf, "<html><head><title></title></head><body><table>");
    int ret = scandir(dirName, &fileList, nullptr, alphasort);
    for (int i = 0; i<ret; ++i){
        char* name = fileList[i]->d_name;
        struct stat st;
        char pathName[512];
        sprintf(pathName, "%s/%s", dirName, name);
        stat(pathName, &st);
        if(S_ISDIR(st.st_mode)){
            sprintf(toSendBuf+strlen(toSendBuf),
             "<tr><td><a href=\"%s/\">%s</a></td><td>%ld</td></tr>", 
             name,  name, st.st_size);
        }else{
            sprintf(toSendBuf + strlen(toSendBuf), 
            "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
             name, name, st.st_size);
        }
        send(curfd, toSendBuf, sizeof(toSendBuf), 0);
        delete fileList[i];
        memset(toSendBuf, 0, sizeof(toSendBuf));
    }
    sprintf(toSendBuf, "</table></body></html>");
    send(curfd, toSendBuf, sizeof(toSendBuf), 0);
    delete [] fileList;
    return true;
}