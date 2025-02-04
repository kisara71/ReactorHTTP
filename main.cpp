#include "Server.h"
#include <stdio.h>
#include "Logger/Logger.h"

int main(){

    Logger::get_instance()->open("./Logger/test.log");
    return 0;
}
