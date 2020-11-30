#include <iostream>
#include "WebServer.h"

int main() {
    try {
        WebServer webServer("0.0.0.0", 8888);
        webServer.start();
    } catch (SocketException &exception) {
        perror(exception.what());
    }

    return 0;
}
