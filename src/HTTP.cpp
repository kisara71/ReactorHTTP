#include <sys/stat.h>
#include <unordered_map>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "HTTP.h"
#include "Logger.h"

const std::unordered_map<int, std::string> statusMsg = {
    {OK, "OK"},
    {BAD_REQUEST, "Bad Request"},
    {NOT_FOUND, "Not Found"},
    {INTERNAL_SERVER_ERROR, "Internal Server Error"}};

int hexToDec(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    return 0;
}

std::string decodeURL(const std::string_view &url)
{
    std::string result;
    result.reserve(url.size());
    for (size_t i = 0; i < url.size(); ++i)
    {
        if (url[i] == '%')
        {
            if (i + 2 < url.size())
            {
                char c1 = hexToDec(url[i + 1]);
                char c2 = hexToDec(url[i + 2]);
                result += (c1 * 16 + c2);
                i += 2;
            }
        }
        else
        {
            result += url[i];
        }
    }
    return result;
}

std::string getFileType(const std::string filename){
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

HTTP::HTTP(int fd) : m_fd(fd),
                     m_fileSize(-1)
{
}

HTTP::~HTTP()
{
}

void HTTP::handleRequest(Buffer *readBuf)
{
    std::string_view sv = readBuf->read();
    debug("read buf: %s", sv.data());
    if (sv.empty())
    {
        debug("read buf is empty");
        return;
    }
    size_t lineEnd = sv.find("\r\n");
    if (lineEnd == std::string_view::npos)
    {
        debug("parse line failed");
        return;
    }
    parseLine(sv.substr(0, lineEnd));

    if (m_method.empty() || m_url.empty())
    {
        debug("parse line failed");
        return;
    }

    size_t headerEnd = sv.find("\r\n\r\n", lineEnd);
    parseHeaders(sv.substr(lineEnd + 2, headerEnd - lineEnd));

    if (m_method[0] == 'P')
    {
        parseContent(sv.substr(headerEnd + 4));
    }
}

void HTTP::parseLine(std::string_view r_line)
{
    size_t methodEnd = r_line.find(" ");
    if (methodEnd == std::string_view::npos)
    {
        return;
    }
    m_method = std::move(std::string(r_line.substr(0, methodEnd)));

    size_t urlEnd = r_line.find(" ", methodEnd + 1);
    m_url = decodeURL(r_line.substr(methodEnd + 1, urlEnd - methodEnd - 1));

    m_version = std::move(std::string(r_line.substr(urlEnd + 1)));

    return;
}

void HTTP::parseHeaders(std::string_view r_headers)
{
    size_t pos = 0;
    while (pos < r_headers.size())
    {
        size_t lineEnd = r_headers.find("\r\n", pos);
        if (lineEnd == std::string_view::npos)
        {
            break;
        }
        std::string_view line = r_headers.substr(pos, lineEnd - pos);
        size_t colonPos = line.find(":");
        std::string key = std::string(line.substr(0, colonPos));
        std::string value = std::string(line.substr(colonPos + 2));
        m_headers[key] = value;
        pos = lineEnd + 2;
    }
}

void HTTP::handleFile(const char *filename)
{
    debug("handle file");
    int file = open(filename, O_RDONLY);
    if (-1 == file)
    {
        error("open file failed: %s", strerror(errno));
    }
    struct stat st;
    stat(filename, &st);
    off_t offset = 0;
    while (offset < st.st_size)
    {
        int ret = sendfile(m_fd, file, &offset, st.st_size - offset);
        if (-1 == ret)
        {
            error("sendfile failed: %s", strerror(errno));
            close(file);
            return;
        }
    }
    close(file);
}
void HTTP::handleHead(std::string fileType)
{
    m_response.str("");
    debug("handle head");
    debug("m_url:%s, m_method:%s, m_version:%s", m_url.c_str(), m_method.c_str(), m_version.c_str());
    m_response << m_version << " " << m_statusCode << " " << statusMsg.at(m_statusCode) << "\r\n";
    m_response << "Content-Type: " << fileType << "\r\n";
    m_response << "Content-Length: " << m_fileSize << "\r\n\r\n";
    send(m_fd, m_response.str().c_str(), m_response.str().size(), 0);
}
void HTTP::handleDir(const char *dirname)
{
    debug("handle dir");
    struct dirent **fileList;
    char toSendBuf[4096] = {0};
    sprintf(toSendBuf, "<html><head><title></title></head><body><table>");
    int ret = scandir(dirname, &fileList, nullptr, alphasort);
    for (int i = 0; i < ret; ++i)
    {
        char *name = fileList[i]->d_name;
        struct stat st;
        char pathName[512];
        sprintf(pathName, "%s/%s", dirname, name);
        stat(pathName, &st);
        if (S_ISDIR(st.st_mode))
        {
            sprintf(toSendBuf + strlen(toSendBuf),
                    "<tr><td><a href=\"%s/\">%s</a></td><td>%ld</td></tr>",
                    name, name, st.st_size);
        }
        else
        {
            sprintf(toSendBuf + strlen(toSendBuf),
                    "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
                    name, name, st.st_size);
        }
        send(m_fd, toSendBuf, sizeof(toSendBuf), 0);
        delete fileList[i];
        memset(toSendBuf, 0, sizeof(toSendBuf));
    }
    sprintf(toSendBuf, "</table></body></html>");
    send(m_fd, toSendBuf, sizeof(toSendBuf), 0);
    delete[] fileList;
    return;
}
void HTTP::handleResponse()
{
    debug("handle response");
    const char *filename = nullptr;
    if (m_url == "/")
    {
        filename = "index.html";
    }
    else
    {
        filename = m_url.c_str() + 1;
    }
    debug("filename: %s", filename);
    struct stat st;
    if (-1 == stat(filename, &st))
    {
        debug("stat file failed: %s", strerror(errno));
        filename = "./404.html";
        m_statusCode = NOT_FOUND;
        struct stat st;
        stat(filename, &st);
        m_fileSize = st.st_size;
        handleHead(getFileType(filename));
        handleFile(filename);
        return;
    }
    m_statusCode = OK;
    if (S_ISDIR(st.st_mode))
    {
        debug("is dir");
        handleHead(getFileType(filename));
        handleDir(filename);
    }
    else
    {
        debug("is file");
        m_fileSize = st.st_size;
        //m_fileType = getFileType(filename);
        handleHead(getFileType(filename));
        handleFile(filename);
    }
    return;
}
void HTTP::parseContent(std::string_view r_content)
{
}