#include "Logger/Logger.h"
#include "Config/Config.h"
#include "TCPServer/TCPServer.h"

#include <thread>


int main(int argc, char* argv[]){

   Config config;
   config.parse_config(argc, argv);
   Logger::get_instance()->log_init(config.logLevel, "./log.log");

   TCPServer* server = new TCPServer(1,55655, DispatcherType::EPOLL);
   server->run();
   
   return 0;
}
