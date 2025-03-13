#ifndef CHANNEL_H
#define CHANNEL_H
enum Events{
        TIMEOUT = 0x00,
        READEVENT = 0x01,
        WRITEEVENT = 0x02
};

class Channel{

public:
    Channel();
    ~Channel();

public:
    void setWriteEvent(bool flag);
    void setReadEvent(bool flag);
public:
    
    int m_fd;
    int m_events;
private:

};
#endif