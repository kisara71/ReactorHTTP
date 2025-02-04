#include "Logger.h"

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
    m_filename = filename;
    m_fout.open(filename, std::ios::app);
    if(m_fout.fail()){
        throw std::runtime_error("open file failed"+filename);
    }
}

void Logger::close(){
    m_fout.close();
}
Logger::Logger(){
    
}
Logger::~Logger(){

}