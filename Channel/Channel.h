#ifndef CHANNEL_H
#define CHANNEL_H
class TCPConnection;

#include <functional>
#include <cstdint>
enum Events: uint8_t{
        TIMEOUT = 0x01,
        READEVENT = 0x02,
        WRITEEVENT = 0x04,
        UNSET = 0x00
};

using handleFunc = std::function<void(void)>;
class Channel{

public:
    Channel(int fd, uint8_t events, TCPConnection* conn = nullptr, handleFunc readCallBack = nullptr, handleFunc writeCallBack = nullptr, handleFunc deleteCallBack = nullptr);
    ~Channel();

public:
    void setWriteEvent(bool flag);
    void setReadEvent(bool flag);

    void defaultReadCallBack();

    handleFunc m_readCallBack;
    handleFunc m_writeCallBack;
    handleFunc m_deleteCallBack;
public:
    
    int m_fd;
    uint8_t m_events;

    TCPConnection* m_tcpCon;

};
#endif