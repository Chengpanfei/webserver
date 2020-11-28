//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_HTTPDECODER_H
#define WEBSERVER_HTTPDECODER_H


#include "../Handler.h"
#include "HttpRequest.h"
#include <unordered_map>

class HttpDecoder : public Handler {
private:
    unordered_map<int, HttpRequest> requestMap;

    static void parseRequestLine(HttpRequest &request, const char *lineBegin, const char *lineEnd);

public:
    HandlerPropagate handle(Message *msg, Socket &socket, Message **result) override;

    void onClose(Socket &socket) override;

};


#endif //WEBSERVER_HTTPDECODER_H
