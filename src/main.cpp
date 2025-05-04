#include "Logger.h"
#include "Config.h"
#include "TCPServer.h"
#include <signal.h>
#include <thread>


int main(int argc, char* argv[]){

   signal(SIGPIPE, SIG_IGN);
   Config config;
   config.parse_config(argc, argv);
   Logger::get_instance()->log_init(config.logLevel, "./log.log");

   TCPServer* server = new TCPServer(10,55655, DispatcherType::EPOLL);
   server->run();
   
   return 0;
}
