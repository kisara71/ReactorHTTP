#include "Server.h"
#include <stdio.h>
#include "Logger/Logger.h"

int main(){

    Logger::get_instance()->open("./Logger/test.log");
    debug("test1");
    info("test22");
    return 0;
}
