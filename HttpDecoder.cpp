//
// Created by cheng on 2020/11/27.
//

#include <algorithm>
#include "HttpDecoder.h"
#include "HttpRequest.h"

bool HttpDecoder::handle(Message *msg, Socket &socket, Message *result) {
    if(msg != nullptr) return false;

    ByteBuffer &inBuffer = socket.getInBuffer();

    char *lineEnd; // 指向行尾
    while ((lineEnd = find(inBuffer.begin(), inBuffer.end(), '\r')) != inBuffer.end()
           && *(lineEnd + 1) == '\n') {
        unsigned int lineLength = lineEnd - inBuffer.begin() + 2;
        write(STDOUT_FILENO, inBuffer.begin(), lineLength);
        inBuffer.fetchNBytes(lineLength);
    }

    result = new HttpRequest();
    return false;
}
