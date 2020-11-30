//
// Created by cheng on 2020/11/28.
//

#ifndef WEBSERVER_HTTPPROCESSOR_H
#define WEBSERVER_HTTPPROCESSOR_H


#include "../Handler.h"
#include "HttpResponse.h"

class HttpProcessor: public Handler {
private:
    unordered_map<int, HttpResponse > responseMap;

public:
    HandlerPropagate handle(Message *msg, Socket &socket, Message **result) override;

    HandlerPropagate onComplete(Socket &socket) override;

    void onClose(Socket &socket) override;
};


#endif //WEBSERVER_HTTPPROCESSOR_H
