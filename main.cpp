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
int main(int argc, char* argv[]){

   //Server ser;
   Config config;
   config.parse_config(argc, argv);
   Logger::get_instance()->log_init(config.logLevel, "./log.log");
   //ser.initServer(config.port);
   //ThreadPool tp;
   {
      ThreadPool(3);
      getchar();
   }
   getchar();
   return 0;
}
