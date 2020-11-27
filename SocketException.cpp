//
// Created by cheng on 2020/11/27.
//

#include "SocketException.h"

SocketException::SocketException(const string &&str) : reason(str) {}

const char *SocketException::what() const noexcept {
    return reason.c_str();
}
