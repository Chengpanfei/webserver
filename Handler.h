//
// Created by cheng on 2020/11/27.
//

#ifndef WEBSERVER_HANDLER_H
#define WEBSERVER_HANDLER_H


#include "Socket.h"
#include "Message.h"

enum HandlerPropagate {
    STOP, NEXT, REVERSE
};

class Handler {
public:
    /*
     * 消息处理器：
     * msg表示上级处理器传递过来的参数
     * socket表示当前连接
     * result表示该处理器要返回给下级的参数
     *
     * 返回值： NEXT 需要传递给下级处理器， STOP不需要传递给下级， REVERSE 向相反方向传递
     */
    virtual HandlerPropagate handle(Message *msg, Socket &socket, Message **result) = 0;

    /*
     * 当EventLoop写完OutBuffer中的消息时，会调用各个处理器的该方法，
     * 以便处理器进行资源的重置等操作。
     */
    virtual void onComplete(Socket &socket) {}

    /*
     * 当EventLoop即将关闭一个连接时，会调用各个处理器的该方法
     * 以便处理器进行资源的释放等操作
     */
    virtual void onClose(Socket &socket) {}
};


#endif //WEBSERVER_HANDLER_H
