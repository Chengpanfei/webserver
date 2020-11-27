//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_HANDLER_H
#define WEBSERVER_HANDLER_H


#include "Socket.h"
#include "Message.h"

class Handler {
public:
    virtual bool handle(Message *msg, Socket &socket, Message *result) = 0;
};


#endif //WEBSERVER_HANDLER_H
