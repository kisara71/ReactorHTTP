#ifndef CHANNEL_H
#define CHANNEL_H
class Channel{
public:
    Channel();
    ~Channel();

public:
    void setWriteEvent(bool flag);
    bool hasWriteEvent();
    
public:
    enum Events{
        TIMEOUT = 0x00,
        READEVENT = 0x01,
        WRITEEVENT = 0x02
    };

    int m_fd;
    int m_events;
private:

};
#endif