#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <unordered_map>

#include "../Channel/Channel.h"
#include "../Logger/Logger.h"
class Dispatcher{
public:
    Dispatcher(std::unordered_map<int, Channel>& map):channelMap(map){};
    virtual ~Dispatcher(){};

protected:
    std::unordered_map<int, Channel>& channelMap;
public:
    virtual bool add(Channel& channel) = 0;
    virtual bool remove(Channel& channel) = 0;
    virtual bool modify(Channel& channel) = 0;
    virtual void dispatch(int timeout) = 0;
    virtual bool clear() = 0;
};

#endif