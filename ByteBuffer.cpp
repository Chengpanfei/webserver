//
// Created by cheng on 2020/11/27.
//

#include "ByteBuffer.h"

void ByteBuffer::fill(const char *buf, unsigned int size) {
    while (true) {
        if (writableBytes() >= size) {
            memcpy(writeIndex, buf, size);
            writeIndex += size;
            return;
        } else if (writableBytes() + unusedBytes() >= size) {
            unsigned int readableNum = readableBytes();
            memcpy(begin_of_storage, readIndex, readableNum);
            readIndex = begin_of_storage;
            writeIndex = readIndex + readableNum;
            return;
        } else {
            resize();
        }
    }

}

int ByteBuffer::fetch(int fd) {
    if (readableBytes() == 0) return 0;
    int numWrite = write(fd, readIndex, readableBytes());

    if (numWrite == -1) return numWrite;

    readIndex += numWrite;
    return numWrite;
}

int ByteBuffer::fill(int fd) {
    unsigned int writableBytes = end_of_storage - writeIndex;
    unsigned int unusedBytes = readIndex - begin_of_storage;
    unsigned int readableBytes = writeIndex - readIndex;
    // 还有可写的空间
    if (writableBytes > 0) {
    } else if (unusedBytes > 0) {// 还有未使用的空间
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

void ByteBuffer::resize() {
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
