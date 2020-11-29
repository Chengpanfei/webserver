//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_BYTEBUFFER_H
#define WEBSERVER_BYTEBUFFER_H


#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include "SocketException.h"
#include <iostream>

class ByteBuffer final {
private:
    const static int INITIAL_CAPACITY = 1024;
    const static int MAX_CAPACITY = 4096;

    char *begin_of_storage;
    char *end_of_storage;

    char *readIndex;
    char *writeIndex;

    ByteBuffer(const ByteBuffer &buffer);

    ByteBuffer &operator=(const ByteBuffer &buffer);

    void resize();

public:
    ByteBuffer() {
        begin_of_storage = (char *) malloc(INITIAL_CAPACITY);
        end_of_storage = begin_of_storage + INITIAL_CAPACITY;
        readIndex = begin_of_storage;
        writeIndex = begin_of_storage;
    }

    ~ByteBuffer() {
        free(begin_of_storage);
    }

    char *data() {
        return readIndex;
    }

    unsigned int writableBytes() {
        return end_of_storage - writeIndex;
    }

    unsigned int unusedBytes() {
        return readIndex - begin_of_storage;
    }

    unsigned int readableBytes() {
        return writeIndex - readIndex;
    }

    char *begin() {
        return readIndex;
    }

    char *end() {
        return writeIndex;
    }

    void fetchNBytes(unsigned int n) {
        readIndex += n;
    }

    void fill(const char *buf, unsigned int size);

    int fetch(int fd);

    int fill(int fd);

};


#endif //WEBSERVER_BYTEBUFFER_H
