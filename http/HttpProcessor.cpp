//
// Created by cheng on 2020/11/28.
//

#include "HttpProcessor.h"
#include "HttpRequest.h"
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

HandlerPropagate HttpProcessor::handle(Message *msg, Socket &socket, Message **result) {
    auto requestPtr = dynamic_cast<HttpRequest *>(msg);
    if (requestPtr == nullptr) return HandlerPropagate::NEXT;


    HttpRequest &request = *requestPtr;
    HttpResponse &response = responseMap[socket.getFd()];

    string filename = "." + request.getPath();

    struct stat st = {};
    int err = stat(filename.c_str(), &st);

    if (err == -1) {
        if (errno == ENOENT) {
            response.setCode("404");
            response.setMsg("Not Found");
            response.setContent("Sorry, couldn't find the file!");
        } else {
            // 无法读取文件
            throw SocketException("读取文件出错！");
        }
    }

    else if (S_ISDIR(st.st_mode)) {
        // 请求地址是目录
        DIR *dir = opendir(filename.c_str());
        if (dir == nullptr) {
            // 出错处理
            throw SocketException("读取文件出错！");
        }
        dirent *dp;
        string content("<html><body><ul>");
        while ((dp = readdir(dir)) != nullptr) {

            // 目录文件需要在末尾加斜杠
            string dpName(dp->d_name);
            if (dp->d_type == DT_DIR)dpName.append("/");

            content.append("<li><a href=\"./").append(dpName)
                    .append("\">").append(dpName).append("</a></li>");
        }
        closedir(dir);
        content.append("</ul></body></html>");
        response.setContent(content);
        response.setHeader("Content-Type", "text/html");
    } else {
        // 非目录文件
        response.setSendFileOn(true);
        response.setSendFileName(filename);
        response.setContentLength(st.st_size);
    }


    *result = &response;
    return HandlerPropagate::REVERSE;
}

HandlerPropagate HttpProcessor::onComplete(Socket &socket) {
    HttpResponse &response = responseMap[socket.getFd()];
    if (response.isSendFileOn()) {
        int fd = open(response.getSendFileName().c_str(), O_RDONLY);
        sendfile(socket.getFd(), fd, nullptr, response.getContentLength());
        close(fd);
    }
    response.reset();
    return HandlerPropagate::NEXT;
}

void HttpProcessor::onClose(Socket &socket) {
    responseMap.erase(socket.getFd());
}
