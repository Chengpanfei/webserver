//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_EVENTLOOP_H
#define WEBSERVER_EVENTLOOP_H


#include "ServerSocket.h"
#include "Socket.h"
#include "Handler.h"
#include <sys/epoll.h>
#include <list>

class EventLoop {
private:
    int epoll_fd;
    int maxEventNum = 100;
    ServerSocket serverSocket;
    Socket *serverPtr{nullptr};
    epoll_event *events{nullptr};

    list<Handler *> inHandlers;
    list<Handler *> outHandlers;

    void handleAcceptEvent();

    void handleReadEvent(Socket *socketPtr);

public:
    EventLoop(const string &host, const unsigned short &port);

    ~EventLoop() {
        if (events) free(events);
        if (serverPtr) free(serverPtr);
        for (auto handler:inHandlers)free(handler);
        for (auto handler:outHandlers)free(handler);
    }

    void registerInHandler(Handler *handler) {
        inHandlers.push_back(handler);
    }

    void registerOutHandler(Handler *handler) {
        outHandlers.push_front(handler);
    }


    void startLoop();;

    void closeConnection(Socket *socketPtr);

    void handleWriteEvent(Socket *socketPtr);
};


#endif //WEBSERVER_EVENTLOOP_H
