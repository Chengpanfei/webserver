//
// Created by cheng on 2020/11/28.
//

#ifndef WEBSERVER_HTTPENCODER_H
#define WEBSERVER_HTTPENCODER_H


#include "../Handler.h"

class HttpEncoder : public Handler {
public:
    HandlerPropagate handle(Message *msg, Socket &socket, Message **result) override;
};


#endif //WEBSERVER_HTTPENCODER_H
