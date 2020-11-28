//
// Created by cheng on 2020/11/28.
//

#include "HttpEncoder.h"
#include "HttpResponse.h"

HandlerPropagate HttpEncoder::handle(Message *msg, Socket &socket, Message **result) {
    auto responsePtr = dynamic_cast<HttpResponse *>(msg);
    if (responsePtr == nullptr) return NEXT;

    ByteBuffer &outBuffer = socket.getOutBuffer();
    char space[1] = {' '}, lineFeed[2] = {'\r', '\n'}, colon[1] = {':'};

    outBuffer.fill(responsePtr->getVersion().data(), responsePtr->getVersion().size());
    outBuffer.fill(space, 1);

    outBuffer.fill(responsePtr->getCode().data(), responsePtr->getCode().size());
    outBuffer.fill(space, 1);

    outBuffer.fill(responsePtr->getMsg().data(), responsePtr->getMsg().size());
    outBuffer.fill(lineFeed, 2);

    for (auto &item: responsePtr->getHeaders()) {
        outBuffer.fill(item.first.data(), item.first.size());
        outBuffer.fill(colon, 1);
        outBuffer.fill(item.second.data(), item.second.size());
        outBuffer.fill(lineFeed, 2);
    }

    outBuffer.fill(lineFeed, 2);

    if (!responsePtr->isSendFileOn()) {
        outBuffer.fill(responsePtr->getContentPtr(), responsePtr->getContentLength());
    }
    return NEXT;
}


