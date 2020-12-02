//
// Created by cheng on 2020/11/27.
//

#include "WebServer.h"

void WebServer::start() {
    // 屏蔽掉sigpipe的默认处理
    signal(SIGPIPE, SIG_IGN);
    Logger::info("signal pipe is ignored!");
    // 注册处理器
    eventLoop.registerInHandler(new HttpDecoder());
    eventLoop.registerInHandler(new HttpProcessor());
    eventLoop.registerOutHandler(new HttpEncoder());
    // 开始事件循环
    Logger::debug("starting eventLoop");
    eventLoop.startLoop();
}
