#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <unordered_map>
#include <sstream>
#include "Buffer.h"

enum PARSE_STATE{
    PARSE_LINE = 0,
    PARSE_HEADER,
    PARSE_CONTENT
};

enum STATUSCODE:int{
        OK = 200,
        BAD_REQUEST = 400,
        NOT_FOUND = 404,
        INTERNAL_SERVER_ERROR = 500
 };


class HTTP
{
public:
    HTTP(int fd);
    ~HTTP();
    void handleRequest(Buffer* readBuf);
    void handleResponse();

private:
    void parseLine(std::string_view r_line);
    void parseHeaders(std::string_view r_headers);
    void parseContent(std::string_view r_content);
    void handleFile(const char* filename);
    void handleDir(const char*  dirname);
    void handleHead(std::string fileType);

private:
    int m_fd;
    std::string m_method;
    std::string m_url;
    std::string m_content;
    std::string m_version;
    size_t m_fileSize;
    std::ostringstream m_response;
    int m_statusCode;
    std::unordered_map<std::string, std::string> m_headers;
};


#endif