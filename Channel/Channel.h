#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <cstdint>
enum Events: uint8_t{
        TIMEOUT = 0x00,
        READEVENT = 0x01,
        WRITEEVENT = 0x02,
        UNSET = 0x04
};

class Channel{

public:
    Channel(uint8_t events= 0x04);
    ~Channel();

public:
    void setWriteEvent(bool flag);
    void setReadEvent(bool flag);
    void setUseless(bool flag);
    void bindReadCallBack(std::function<bool(void)> func);
    void bindWriteCallBack(std::function<bool(void)> func);

    std::function<bool(void)> readCallBack;
    std::function<bool(void)> writeCallBack;
public:
    
    int m_fd;
    uint8_t m_events;
private:
    bool USELESS;

};
#endif