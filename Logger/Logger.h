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
    void log_init(Level level,const std::string filename="log.log", int maxBytes=0);
    void open(const std::string& filename);


private:
    Logger();
    ~Logger();
    void rotate();
    void close();
    void set_level(Level level);


private:
    int m_max;
    int m_len;
    static Logger *m_instance; // logger实例指针
    std::ofstream m_fout;            // 文件指针
    std::string m_filename;
    static const char *s_level[LEVEL_COUNT];
    Level m_level;
};
#define debug(format, ...) Logger::get_instance()->log(Logger::Level::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define info(format, ...) Logger::get_instance()->log(Logger::Level::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define warn(format, ...) Logger::get_instance()->log(Logger::Level::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define error(format, ...) Logger::get_instance()->log(Logger::Level::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define fatal(format, ...) Logger::get_instance()->log(Logger::Level::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);
#endif