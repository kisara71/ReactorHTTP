#include <cstring>
#include <sys/uio.h>
#include <string.h>

#include "../Logger/Logger.h"
#include "Buffer.h"

Buffer::Buffer(int capacity) noexcept:
m_data(new char[capacity]),
m_readPos(0),
m_writePos(0),
m_readable(0),
m_writeable(0),
m_capacity(capacity)
{


}

Buffer::~Buffer() noexcept
{
    if(m_data!=nullptr)
    {
        delete m_data;
    }
}

void Buffer::extenCapRoom(int& size) noexcept
{
    if(m_writeable > size)
    {
        return;
    }else if(m_readPos + m_writeable > size)
    {
        memcpy(m_data, m_data + m_readPos, m_readable);
        m_writePos   = m_readable;
        m_writeable += m_readPos;
        m_readPos    = 0;
        return;
    }else{
        char* temp = new char[m_capacity*2];
        memcpy(temp, m_data + m_readPos, m_capacity - m_readPos);
        m_capacity = m_capacity*2;
        m_writePos = m_readable;
        m_writeable = m_capacity - m_readable;
        m_readPos = 0;
        delete m_data;
        m_data = temp;
        return;
    }
}

void Buffer::write(const char* data) noexcept
{
    int size = strlen(data);
    extenCapRoom(size);
    memcpy(m_data, m_data + m_writePos, size);
    m_readable += size;
    m_writeable -= size;
    m_writePos += size;
    return;
}

bool Buffer::readFromFD(int fd)
{
    iovec vec[2];
    vec[0].iov_base = m_data + m_writePos;
    vec[1].iov_len = m_writeable;
    char gourdBuf[10240];
    vec[1].iov_base = gourdBuf;
    vec[1].iov_len = 10240;

    int ret = readv(fd, vec, 2);
    if(-1 == ret)
    {
        error("read from fd: %d failed: %s", fd, strerror(errno));
        return false;
    }else if(ret <= m_writeable)
    {
        m_writeable -= ret;
        m_writePos += ret;
        debug("read from fd: %d with %d byte", fd, ret);
        return true;
    }else {
        m_writePos += ret;
        m_writeable -= ret;
        write(gourdBuf);
        debug(" write into buffer from gourdBuf");
        return true;
    }
}