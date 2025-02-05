#include "Server/Server.h"
#include <stdio.h>
#include "Logger/Logger.h"
#include "Config/Config.h"
int main(int argc, char* argv[]){

   Server ser;
   Config config;
   config.parse_config(argc, argv);
   Logger::get_instance()->log_init(config.logLevel);
   ser.initServer(config.port);
    return 0;
}
