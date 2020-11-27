//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_HTTPDECODER_H
#define WEBSERVER_HTTPDECODER_H


#include "Handler.h"

class HttpDecoder: public Handler {
public:
    virtual bool handle(Message *msg, Socket &socket, Message *result) override;
};


#endif //WEBSERVER_HTTPDECODER_H
