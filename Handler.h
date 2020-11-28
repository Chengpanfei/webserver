//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_HANDLER_H
#define WEBSERVER_HANDLER_H


#include "Socket.h"
#include "Message.h"

enum HandlerPropagate {
    STOP, NEXT, REVERSE
};

class Handler {
public:
    virtual HandlerPropagate handle(Message *msg, Socket &socket, Message **result) = 0;

    virtual void onComplete(Socket &socket) {}

    virtual void onClose(Socket &socket) {}
};


#endif //WEBSERVER_HANDLER_H
