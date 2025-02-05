#ifndef CONFIG_H
#define CONFIG_H
#include "../Logger/Logger.h"
#include <string>
class Config{
public:
    Config();
    ~Config();

    void parse_config(int argc, char* argv[]);
public:
    unsigned short port;
    Logger::Level logLevel;

};

#endif