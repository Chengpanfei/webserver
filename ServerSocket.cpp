//
// Created by cheng on 2020/11/27.
//

#include "ServerSocket.h"


ServerSocket::ServerSocket(const string &host, const unsigned short &port)
        : host(host), port(port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) throw SocketException("socket 调用出错！");
    struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = {htonl(inet_addr(host.c_str()))},
    };

    if (bind(socket_fd, (sockaddr *) &addr, sizeof(addr)) == -1) {
        throw SocketException("bind 出错！");
    }

    if (listen(socket_fd, backlog) == -1) {
        throw SocketException("Listen 出错！");
    }
    Logger::info("Listening: %s : %h   fd: %d", host.c_str(), port, socket_fd);

    // 设置为非阻塞
    int flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

    this->fd = socket_fd;
}

void ServerSocket::stop() {
    if (fd) {
        close(fd);
    }
}

const string &ServerSocket::getHost() const {
    return host;
}

unsigned short ServerSocket::getPort() const {
    return port;
}

int ServerSocket::getFd() const {
    return fd;
}
