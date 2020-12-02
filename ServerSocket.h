//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_SERVERSOCKET_H
#define WEBSERVER_SERVERSOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "SocketException.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include "Logger.h"

using namespace std;

class ServerSocket {
private:
    string host;
    unsigned short port;
    int fd;
    int backlog = 10;

public:
    ServerSocket(const string &host, const unsigned short &port);

    void stop();

    const string &getHost() const;

    unsigned short getPort() const;

    int getFd() const;
};


#endif //WEBSERVER_SERVERSOCKET_H
