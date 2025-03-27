#ifndef BUFFER_H
#define BUFFER_H

class Buffer{

public:
    Buffer(int capacity= 2048) noexcept;
    ~Buffer() noexcept;
public:
    void write(const char* data) noexcept;
    bool readFromFD(int fd);
private:
    void extenCapRoom(int& size) noexcept;

private:
    char* m_data;
    int m_readPos;
    int m_writePos;
    int m_readable;
    int m_writeable;
    int m_capacity;
};


#endif