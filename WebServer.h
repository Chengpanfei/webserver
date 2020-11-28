//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_WEBSERVER_H
#define WEBSERVER_WEBSERVER_H

#include <string>
#include "EventLoop.h"
#include "http/HttpDecoder.h"
#include "http/HttpEncoder.h"
#include "http/HttpProcessor.h"

using namespace std;

class WebServer {
private:
    unsigned short port = 80;
    string host = "localhost";
    EventLoop eventLoop;

public:
    WebServer(const string &host, const unsigned short &port)
            : port(port), host(host), eventLoop(host, port) {};

    void start();;

    void stop();
};


#endif //WEBSERVER_WEBSERVER_H
