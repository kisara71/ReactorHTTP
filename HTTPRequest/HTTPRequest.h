#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
class HTTPRequest{
public:
    HTTPRequest();
    ~HTTPRequest();

public:
    void init(int epfd);
    bool recvHTTPRequest(int curfd);
    void parseHTTPRequest(const char* request);

public:
    struct httpINFO{
        char method[12];
        char requestLine[1024];
    };
private:
    int m_epfd;
};

#endif