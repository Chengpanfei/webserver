//
// Created by cheng on 2020/11/27.
//

#include <algorithm>
#include "HttpDecoder.h"

HandlerPropagate HttpDecoder::handle(Message *msg, Socket &socket, Message **result) {
    if (msg != nullptr) return HandlerPropagate::NEXT;

    ByteBuffer &inBuffer = socket.getInBuffer();
    HttpRequest &request = requestMap[socket.getFd()];

    char *lineEnd; // 指向行尾
    while ((lineEnd = find(inBuffer.begin(), inBuffer.end(), '\r')) != inBuffer.end()
           && *(lineEnd + 1) == '\n') {

        if (lineEnd == inBuffer.begin()) {
            inBuffer.fetchNBytes(2);
            request.setStatus(ParseState::EXPECT_REQUEST_LINE);
            *result = &request;
            return HandlerPropagate::NEXT;
        } else if (request.getStatus() == ParseState::EXPECT_REQUEST_LINE) {
            // 解析请求行
            parseRequestLine(request, inBuffer.begin(), lineEnd);
        } else if (request.getStatus() == ParseState::PARSING_HEADER) {
            auto colon_pos = find(inBuffer.begin(), lineEnd, ':');
            string key(inBuffer.begin(), colon_pos);
            string val(colon_pos + 1, lineEnd);
            request.setHeader(key, val);
        } else {

        }
        unsigned int lineLength = lineEnd - inBuffer.begin() + 2;
        write(STDOUT_FILENO, inBuffer.begin(), lineLength);
        inBuffer.fetchNBytes(lineLength);
    }

    return HandlerPropagate::STOP;
}

void HttpDecoder::parseRequestLine(HttpRequest &request,
                                   const char *lineBegin, const char *lineEnd) {
    // 获取请求方法
    auto first_space = find(lineBegin, lineEnd, ' ');
    string method(lineBegin, first_space);
    // 获取请求路径和queryString
    auto second_space = find(first_space + 1, lineEnd, ' ');
    string url(first_space + 1, second_space);
    auto question_pos = find(url.begin(), url.end(), '?');
    string path(url.begin(), question_pos);
    if (question_pos != url.end()) {
        string query(question_pos + 1, url.end());
        request.setQuery(query);
    }
    // 获取版本号
    string version(second_space + 1, lineEnd);

    request.setMethod(method);
    request.setPath(path);
    request.setVersion(version);

    request.setStatus(ParseState::PARSING_HEADER);
}

void HttpDecoder::onClose(Socket &socket) {
    requestMap.erase(socket.getFd());
}
