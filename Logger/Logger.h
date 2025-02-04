#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <stdexcept>
#include <fstream>
class Logger
{

public:
    enum Level{
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
    }; // 日志等级

    static Logger *get_instance(); // 单例模式

    void log(Level level, const char *filename, int line, const char *format, ...);
    void open(const std::string& filename);
    void close();

private:
    Logger();
    ~Logger();

private:
    static Logger *m_instance; // logger实例指针
    std::ofstream m_fout;            // 文件指针
    std::string m_filename;
    static const char *s_level[LEVEL_COUNT];
};

#endif