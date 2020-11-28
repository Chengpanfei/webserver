//
// Created by cheng on 2020/11/27.
//

#include "EventLoop.h"

void EventLoop::handleServerSocketEvent() {
    // 接收客户端地址信息
    sockaddr_in client_addr = {};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(serverSocket.getFd(), (sockaddr *) &client_addr, &client_addr_len);

    if (client_fd == -1) {
        if (errno == EAGAIN) return;
        throw SocketException("accept 出错");
    }

    // 为client socket分配空间
    auto *ptr = new Socket(inet_ntoa(client_addr.sin_addr), client_addr.sin_port, client_fd);
    epoll_event ev = {
            .events = EPOLLIN,
            .data={ptr}
    };

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
        throw SocketException("epoll add 失败");
    }
}

void EventLoop::handleConnectSocketEvent(Socket *socketPtr) {
    unsigned int numRead = socketPtr->fillInBuffer();
    if (numRead == 0) {
        // 读取到socket末尾
        closeConnection(socketPtr);
        return;
    }
    // callback
    Message *result, **resultPtr = &result, *param = nullptr;
    HandlerPropagate propagateType;

    // 正向传播
    for (Handler *handler:inHandlers) {
        propagateType = handler->handle(param, *socketPtr, resultPtr);
        param = *resultPtr;
        if (propagateType != HandlerPropagate::NEXT) {
            break;
        }
    }

    // 反向传播
    if (propagateType == HandlerPropagate::REVERSE) {
        for (Handler *handler: outHandlers) {
            handler->handle(param, *socketPtr, resultPtr);
            param = *resultPtr;
        }

        // 反向传播完成， 返回数据就会写入到outBuffer, 准备写入socketBuffer
        epoll_event ev = {.events=EPOLLOUT, .data={.ptr=socketPtr}};
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, socketPtr->getFd(), &ev);
    }


}

void EventLoop::closeConnection(Socket *socketPtr) {
    // 发送关闭信号， 让handlers有机会释放占用空间
    for (Handler *handler:inHandlers) handler->onClose(*socketPtr);
    for (Handler *handler:outHandlers)handler->onClose(*socketPtr);

    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socketPtr->getFd(), nullptr) == -1) {
        throw SocketException("Epoll del 出错！");
    }
    close(socketPtr->getFd());
    delete socketPtr;
    clog << "客户端关闭连接！fd:" << socketPtr->getFd() << endl;

}

EventLoop::EventLoop(const string &host, const unsigned short &port)
        : serverSocket(host, port) {
    // 分配空间
    events = (epoll_event *) malloc(sizeof(epoll_event) * maxEventNum);

    epoll_fd = epoll_create(1);
    if (epoll_fd == -1) {
        throw SocketException("epoll创建失败！");
    }
    clog << "epoll创建成功， fd：" << epoll_fd << endl;

    auto *ptr = new Socket(serverSocket);
    epoll_event ev = {.events = EPOLLIN, .data = {ptr}};
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket.getFd(), &ev)) {
        throw SocketException("监听服务端socket失败！");
    }
    clog << "epoll add server成功， fd：" << epoll_fd << endl;

}

void EventLoop::startLoop() {
    while (true) {
        clog << "epoll waiting....fd:" << epoll_fd << endl;
        int nfd = epoll_wait(epoll_fd, events, maxEventNum, -1);
        if (nfd == -1) {
            if (errno == EAGAIN) {
                // 发生惊群效应
                continue;
            }
            throw SocketException("epoll wait 出错！");
        }

        for (int i = 0; i < nfd; ++i) {
            auto *socketPtr = (Socket *) events[i].data.ptr;
            if (socketPtr->getFd() == serverSocket.getFd()) {
                // 处理accept事件
                handleServerSocketEvent();
            } else if (events[i].events == EPOLLIN) {
                // 处理读事件
                handleConnectSocketEvent(socketPtr);
            } else {
                // 处理写事件
                handlerWriteEvent(socketPtr);
            }

        }// for
    }//while
}

void EventLoop::handlerWriteEvent(Socket *socketPtr) {
    int numWrite = socketPtr->fetchOutBuffer();
    if (numWrite == -1) {
        if (errno == EPIPE) {
            // 对方已关闭
            closeConnection(socketPtr);
            return;
        }
        throw SocketException("写数据出错");
    }
    epoll_event ev = {.events=EPOLLIN, .data={.ptr=socketPtr}};
    if (numWrite == 0) {
        // 发送写完事件给handlers， 让handlers有机会处理一些事情
        for (Handler *handler:inHandlers) handler->onComplete(*socketPtr);
        for (Handler *handler:outHandlers)handler->onComplete(*socketPtr);
        // 重新监听可读事件
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, socketPtr->getFd(), &ev);
    }
}
