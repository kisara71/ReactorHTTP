//#include "Server/Server.h"
#include <stdio.h>
#include "Logger/Logger.h"
#include "Config/Config.h"
#include <unistd.h>
#include "ThreadPool/ThreadPool.h"
#include <thread>
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include "TCPServer/TCPServer.h"
#include <thread>

void test(TCPServer* ser)
{
   ser->run();
}
int main(int argc, char* argv[]){

   //Server ser;
   Config config;
   config.parse_config(argc, argv);
   Logger::get_instance()->log_init(config.logLevel, "./log.log");
   //ser.initServer(config.port);
   //ThreadPool tp;
   {
      TCPServer* server = new TCPServer(1,55655, DispatcherType::EPOLL);
      std::thread t(test, server);
      getchar();
      server->stop();
      if(t.joinable())
      {
         t.join();
      }
   }
   getchar();
   return 0;
}
