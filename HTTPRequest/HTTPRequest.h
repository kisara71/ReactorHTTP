#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <string>
struct httpINFO{
    char method[12];
    char requestLine[1024];
 };


class HTTPRequest{
public:
    HTTPRequest();
    ~HTTPRequest();
public:
    enum HTTPStatus{
        HTTP_OK = 200,
        HTTP_NOTFOUND = 404,
        HTTP_BADREQUEST = 400,
        HTTP_FORBIDDEN = 403,
        HTTP_INTERNALERROR = 500,
    };

public:
    void init(int epfd);
    bool recvHTTPRequest(int curfd);
    bool parseHTTPRequest(int curfd, const char* request);
    bool sendHTTPFile(int curfd, const char* filename);
    bool sendHTTPHead(int curfd, HTTPStatus status, const char* type, const char* msg, int size=-1);
    bool sendHTTPDir(int curfd, const char* dirName);
    std::string getFileType(const std::string& filename);
private:
    int m_epfd;
    std::string m_root;
};

#endif