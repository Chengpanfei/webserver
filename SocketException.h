//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_SOCKETEXCEPTION_H
#define WEBSERVER_SOCKETEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class SocketException : public exception {
private:
    string reason;
public:
    SocketException(const string &&str);;

    const char *what() const noexcept override;
};


#endif //WEBSERVER_SOCKETEXCEPTION_H
