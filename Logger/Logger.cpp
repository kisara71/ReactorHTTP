#include "Logger.h"
#include <time.h>
#include <string>
#include <stdarg.h>

//初始化等级和单例指针
Logger* Logger::m_instance = nullptr;
const char* Logger::s_level[LEVEL_COUNT] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};


Logger* Logger::get_instance(){
    if(m_instance == nullptr){
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::open(const std::string& filename){
    close();
    m_filename = filename;
    m_fout.open(filename, std::ios::app);
    if(m_fout.fail()){
        throw std::runtime_error("open file failed"+filename);
    }
    m_fout.seekp(0, std::ios::end);
    m_len = m_fout.tellp();
}

void Logger::close(){
    m_fout.close();
}
Logger::Logger():m_len(0)
{
    log_init("autoCreated.log", DEBUG, 0);
}
Logger::~Logger(){
    close();
}


void Logger::log(Logger::Level level, const char* filename, int line, const char* format, ... ){
    if(m_level>level){
        return;
    }
    //获取当前时间
    time_t t = time(NULL);
    struct tm* timeInfo = localtime(&t);
    char timeBuf[32];
    int size = strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", timeInfo);
    m_len+=size;

    //获取输出内容
    va_list argPtr;
    va_start(argPtr, format);
    size = vsnprintf(NULL, 0, format, argPtr);
    va_end(argPtr);
    char content[size+1];
    if(size>0){
        va_start(argPtr, format);
        vsnprintf(content, size+1, format, argPtr);
        va_end(argPtr);
    }
    m_fout<<'['<<timeBuf<<'|'<<s_level[level]<<"]  "<<filename<<"|"<<line<<":  "<<content<<'\n';
    m_len+=size+10;
    m_fout.flush();
    if(m_len>=m_max&&m_max>0){
        rotate();
    }
    
}
void Logger::set_level(Logger::Level level){
    m_level = level;
}

void Logger::rotate(){
    close();
    time_t tick = time(NULL);
    struct tm* timeInfo = localtime(&tick);
    char timeBuf[32];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", timeInfo);
    std::string newName = m_filename+'.'+timeBuf;
    if(rename(m_filename.c_str(), newName.c_str())!=0){
        throw std::runtime_error("rename log failed");
    }
    open(m_filename);

}
void Logger::log_init(const std::string& filename, Level level, int maxBytes){
    open(filename);
    set_level(level);
    m_max = maxBytes;
}