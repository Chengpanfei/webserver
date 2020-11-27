//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_SOCKET_H
#define WEBSERVER_SOCKET_H

#include <string>
#include "ServerSocket.h"
#include "ByteBuffer.h"

using namespace std;

class Socket {
private:
    string host;
    unsigned int port;
    int fd;
    ByteBuffer inBuffer;
    ByteBuffer outBuffer;
public:
    Socket(char *host_str, const unsigned int &port, const int &fd)
            : host(host_str), port(port), fd(fd) {}

    explicit Socket(ServerSocket &serverSocket)
            : host(serverSocket.getHost()),
              port(serverSocket.getPort()),
              fd(serverSocket.getFd()) {}

    const string &getHost() const {
        return host;
    }

    unsigned int getPort() const {
        return port;
    }

    int getFd() const {
        return fd;
    }

    ByteBuffer &getInBuffer() {
        return inBuffer;
    }
    ByteBuffer &getOutBuffer() {
        return outBuffer;
    }

    unsigned int fillInBuffer(){
        return inBuffer.fill(fd);
    }

    void closeSocket(){
        close(fd);
    }
};


#endif //WEBSERVER_SOCKET_H
