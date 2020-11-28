//
// Created by cheng on 2020/11/28.
//

#include "HttpProcessor.h"
#include "HttpRequest.h"
#include <sys/sendfile.h>
#include <fcntl.h>

HandlerPropagate HttpProcessor::handle(Message *msg, Socket &socket, Message **result) {
    auto requestPtr = dynamic_cast<HttpRequest *>(msg);
    if (requestPtr == nullptr) return HandlerPropagate::NEXT;


    HttpRequest &request = *requestPtr;
    HttpResponse &response = responseMap[socket.getFd()];

    string filename = "." + request.getPath();

    if (access(filename.c_str(), 0) != 0) {
        response.setCode("404");
        response.setMsg("Not Found!");
        response.setContent("Sorry, couldn't find the file!");
    } else {

        response.setSendFileOn(true);
        response.setSendFileName(filename);
        int fd = open(response.getSendFileName().c_str(), O_RDONLY);
        int fileSize = lseek(fd, 0, SEEK_END);
        response.setContentLength(fileSize);
        close(fd);
    }

    *result = &response;
    return HandlerPropagate::REVERSE;
}

void HttpProcessor::onComplete(Socket &socket) {
    HttpResponse &response = responseMap[socket.getFd()];
    if (response.isSendFileOn()) {
        int fd = open(response.getSendFileName().c_str(), O_RDONLY);
        sendfile(socket.getFd(), fd, nullptr, response.getContentLength());
        close(fd);
    }
}

void HttpProcessor::onClose(Socket &socket) {
    responseMap.erase(socket.getFd());
}
