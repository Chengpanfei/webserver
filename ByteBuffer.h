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

    char *begin() {
        return readIndex;
    }

    char *end() {
        return writeIndex;
    }

    void fetchNBytes(unsigned int n) {
        readIndex += n;
    }

    unsigned int fill(int fd) {
        unsigned int writableBytes = end_of_storage - writeIndex;
        unsigned int unusedBytes = readIndex - begin_of_storage;
        unsigned int readableBytes = writeIndex - readIndex;
        // 还有可写的空间
        if (writableBytes > 0) {
        }
        else if (unusedBytes > 0) {// 还有未使用的空间
            memcpy(begin_of_storage, readIndex, readableBytes);
            readIndex = begin_of_storage;
            writeIndex = readIndex + readableBytes;
        } else { // 需要扩容
            resize();
        }

        int numRead = read(fd, writeIndex, end_of_storage - writeIndex);
        writeIndex += numRead;
        return numRead;

    }

    void resize() {
        // 缓冲区已经被填满， 扩容
        unsigned int newSize = (end_of_storage - begin_of_storage) * 2;
        if (newSize > MAX_CAPACITY) {
            throw SocketException("缓冲区以达到上限！");
        }
        begin_of_storage = (char *) realloc(begin_of_storage, newSize);
        end_of_storage = begin_of_storage + newSize;

        unsigned int readableBytes = writeIndex - readIndex;
        readIndex = begin_of_storage;
        writeIndex = readIndex + readableBytes;
        clog << "扩容后：" << newSize << endl;
    }

};


#endif //WEBSERVER_BYTEBUFFER_H
