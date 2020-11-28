//
// Created by cheng on 2020/11/27.
//

#include <algorithm>
#include "HttpDecoder.h"

HandlerPropagate HttpDecoder::handle(Message *msg, Socket &socket, Message **result) {
    if (msg != nullptr) return HandlerPropagate::NEXT;

    ByteBuffer &inBuffer = socket.getInBuffer();
    HttpRequest &request = requestMap[socket.getFd()];

    while (true) {
        if (request.getStatus() == ParseState::PARSING_CONTENT) {
            unsigned int unreceivedLength = request.getContentLength() - request.getReceivedLength();
            unsigned int planToRead = min(inBuffer.readableBytes(), unreceivedLength);

            memcpy(request.getContentPtr() + request.getReceivedLength(), inBuffer.begin(), planToRead);
            inBuffer.fetchNBytes(planToRead);
            request.setReceivedLength(request.getReceivedLength() + planToRead);

            if (request.getReceivedLength() == request.getContentLength()) {
                request.setStatus(ParseState::EXPECT_REQUEST_LINE);
                *result = &request;
                return NEXT;
            }
            break;
        }

        // 不够一行
        char *lineEnd = find(inBuffer.begin(), inBuffer.end(), '\r');
        if (lineEnd == inBuffer.end() || *(lineEnd + 1) != '\n') break;

        if (lineEnd == inBuffer.begin()) {
            // 读取到空白行
            inBuffer.fetchNBytes(2);
            // 没有请求体
            auto lengthPair = request.getHeaders().find("Content-Length");
            if (lengthPair == request.getHeaders().end()) {
                request.setStatus(ParseState::EXPECT_REQUEST_LINE);
                *result = &request;
                return HandlerPropagate::NEXT;
            }

            // TODO 错误处理
            int contentLength = atoi(lengthPair->second.c_str());

            request.setContentPtr((char *) malloc(contentLength));
            request.setContentLength(contentLength);
            request.setReceivedLength(0);
            request.setStatus(ParseState::PARSING_CONTENT);
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

void HttpDecoder::onComplete(Socket &socket) {
    requestMap[socket.getFd()].reset();
}

void HttpDecoder::onClose(Socket &socket) {
    requestMap.erase(socket.getFd());
}
