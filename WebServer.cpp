//
// Created by cheng on 2020/11/27.
//

#include "WebServer.h"

void WebServer::start() {

    // 注册处理器
    eventLoop.registerInHandler(new HttpDecoder());
    eventLoop.registerInHandler(new HttpProcessor());
    eventLoop.registerOutHandler(new HttpEncoder());
    // 开始事件循环
    eventLoop.startLoop();
}
