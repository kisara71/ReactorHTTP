#include "Config.h"
#include <unistd.h>
#include <strings.h>
Config::Config(){
    port = 55655;
    logLevel = Logger::DEBUG;
}

void Config::parse_config(int argc, char* argv[]){
    int opt;
    const char* optStr = "p:l:d:";
    std::string msg;
    while((opt = getopt(argc, argv, optStr))!=-1){
        switch(opt){
            case 'p':
                port = atoi(optarg);
                break;
            case 'l':
                if(strcasecmp("DEBUG", optarg) == 0){
                    logLevel = Logger::DEBUG;
                    break;
                }else  if(strcasecmp("INFO", optarg) == 0){
                    logLevel = Logger::INFO;
                    break;
                }else  if(strcasecmp("WARN", optarg) == 0){
                    logLevel = Logger::WARN;
                    break;
                }else  if(strcasecmp("ERROR", optarg) == 0){
                    logLevel = Logger::ERROR;
                    break;
                }else  if(strcasecmp("FATAL", optarg) == 0){
                    logLevel = Logger::FATAL;
                    break;
                }else{
                    break;
                }
            case 'a':
                chdir(optarg);
                break;
            default:
                break;
        }
    }
}
Config::~Config(){

}