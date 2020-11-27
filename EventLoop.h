//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_EVENTLOOP_H
#define WEBSERVER_EVENTLOOP_H


#include "ServerSocket.h"
#include "Socket.h"
#include "Handler.h"
#include <sys/epoll.h>
#include <vector>

class EventLoop {
private:
    int epoll_fd;
    int maxEventNum = 100;
    ServerSocket serverSocket;
    epoll_event *events = nullptr;

    vector<Handler *> inHandlers;

    void handleServerSocketEvent();

    void handleConnectSocketEvent(Socket *socketPtr);

public:
    EventLoop(const string &host, const unsigned short &port);

    ~EventLoop() {
        if (events) free(events);
        for (auto handler:inHandlers)free(handler);
    }

    void registerInHandler(Handler *handler) {
        inHandlers.push_back(handler);
    }


    void startLoop();

};


#endif //WEBSERVER_EVENTLOOP_H
