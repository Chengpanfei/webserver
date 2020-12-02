#include <iostream>
#include "WebServer.h"


int main() {
    Logger::init("server.log", Logger::Level::WARNING);
    try {
        WebServer webServer("0.0.0.0", 8888);
        webServer.start();
    } catch (SocketException &exception) {
        perror(exception.what());
    }

    return 0;
}
