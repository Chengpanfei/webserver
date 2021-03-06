//
// Created by cheng on 2020/11/23.
//

#ifndef WEBSERVER_HTTPRESPONSE_H
#define WEBSERVER_HTTPRESPONSE_H

#include <string>
#include <unordered_map>
#include "../Message.h"

using namespace std;

class HttpResponse : public Message {
private:
    string version;
    string code;
    string msg;

    bool sendFileOn{};
    string sendFileName;

    unsigned int contentLength{};
    char *contentPtr{nullptr};

    unordered_map<string, string> headers;

    // 禁止拷贝构造
    HttpResponse(const HttpResponse &rep) ;
    // 禁止拷贝赋值
    HttpResponse &operator=(const HttpResponse &rep) ;
public:
    HttpResponse()
            : version("HTTP/1.1"), code("200"),
              msg("OK"), contentPtr(nullptr), contentLength(0) {
        headers["Server"] = "Netty4Cpp";
    }

    ~HttpResponse() override {
        if (contentPtr != nullptr) {
            free(contentPtr);
        }
    }

    const string &getVersion() const;

    void setVersion(const string &version);

    const string &getCode() const;

    void setCode(const string &code);

    const string &getMsg() const;

    void setMsg(const string &msg);

    const unordered_map<string, string> &getHeaders() const;

    void setHeader(const string &key, const string &val);

    void setContent(const string &content);

    unsigned int getContentLength() const;

    void setContentLength(int len);

    char *getContentPtr() const;

    bool isSendFileOn() const;

    void setSendFileOn(bool sendFile);

    const string &getSendFileName() const;

    void setSendFileName(const string &sendFileName);

    void reset() {
        if (contentPtr != nullptr) {
            free(contentPtr);
            contentPtr = nullptr;
        }
        sendFileOn = false;
        msg = "OK";
        code = "200";
    }
};


#endif //WEBSERVER_HTTPRESPONSE_H
