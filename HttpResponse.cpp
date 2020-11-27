//
// Created by cheng on 2020/11/23.
//

#include "HttpResponse.h"

const string &HttpResponse::getVersion() const {
    return version;
}

void HttpResponse::setVersion(const string &version) {
    HttpResponse::version = version;
}

const string &HttpResponse::getCode() const {
    return code;
}

void HttpResponse::setCode(const string &code) {
    HttpResponse::code = code;
}

const string &HttpResponse::getMsg() const {
    return msg;
}

void HttpResponse::setMsg(const string &msg) {
    HttpResponse::msg = msg;
}

const unordered_map<string, string> &HttpResponse::getHeaders() const {
    return headers;
}

void HttpResponse::setHeader(const string &key, const string &val) {
    headers[key] = val;
}

void HttpResponse::setContent(const string &content) {
    if(contentPtr != nullptr){
        free(contentPtr);
    }
    contentPtr = (char *)malloc(content.size());
    copy(content.begin(), content.end(), contentPtr);
    contentLength = content.size();
    setHeader("Content-Length", to_string(contentLength));
    setHeader("Content-Type", "text/plain");
}

unsigned int HttpResponse::getContentLength() const {
    return contentLength;
}

char *HttpResponse::getContentPtr() const {
    return contentPtr;
}

void HttpResponse::setContentLength(int len) {
    contentLength = len;
    setHeader("Content-Length", to_string(len));
}

