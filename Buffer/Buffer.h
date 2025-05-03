#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class Buffer{

public:
    Buffer(int capacity= 2048) noexcept;
    ~Buffer() noexcept;
public:

    void reset() noexcept;
    void write(const char* data) noexcept;
    std::string_view read() noexcept;
    int readFromFD(int fd);
private:
    void extenCapRoom(int& size) noexcept;

public:
    char* m_data;
    int m_readPos;
    int m_writePos;
    int m_readable;
    int m_writeable;
    int m_capacity;
};


#endif