#include "Server/Server.h"
#include <stdio.h>
#include "Logger/Logger.h"
#include "Config/Config.h"
#include <unistd.h>
int main(int argc, char* argv[]){

   Server ser;
   Config config;
   config.parse_config(argc, argv);
   Logger::get_instance()->log_init(config.logLevel, "/home/kisara/vscode/tinyWebserver/Logger/log.log");
   ser.initServer(config.port);
   getchar();
    return 0;
}
