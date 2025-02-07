#include "Server/Server.h"
#include <stdio.h>
#include "Logger/Logger.h"
#include "Config/Config.h"
#include <unistd.h>
int main(int argc, char* argv[]){

   Server ser;
   Config config;
   config.parse_config(argc, argv);
   Logger::get_instance()->log_init(config.logLevel);
   ser.initServer(config.port);
   char buf[512];
   getcwd(buf, sizeof(buf));
   info("current work dir is: %s", buf);
   getchar();
    return 0;
}
