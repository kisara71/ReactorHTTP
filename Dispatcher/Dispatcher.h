#ifndef DISPATCHER_H
#define DISPATCHER_H


#include "../Channel/Channel.h"
#include "../Logger/Logger.h"
class Dispatcher{
public:
    Dispatcher() = default;
    virtual ~Dispatcher() = 0;
public:
    virtual bool add(Channel& channel) = 0;
    virtual bool remove(Channel& channel) = 0;
    virtual bool modify(Channel& channel) = 0;
    virtual bool dispatch(Channel& channel, int timeout) = 0;
};

#endif