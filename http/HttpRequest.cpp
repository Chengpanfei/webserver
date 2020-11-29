//
// Created by cheng on 2020/11/23.
//

#include "HttpRequest.h"

HttpRequest::HttpRequest() :
        status(ParseState::EXPECT_REQUEST_LINE), ip(0), port(0) {}

ParseState HttpRequest::getStatus() const {
    return status;
}

void HttpRequest::setStatus(const ParseState &status) {
    HttpRequest::status = status;
}

const string &HttpRequest::getMethod() const {
    return method;
}

void HttpRequest::setMethod(const string &method) {
    HttpRequest::method = method;
}

const string &HttpRequest::getPath() const {
    return path;
}

void HttpRequest::setPath(const string &path) {
    HttpRequest::path = path;
}

const string &HttpRequest::getQuery() const {
    return query;
}

void HttpRequest::setQuery(const string &query) {
    HttpRequest::query = query;
}

const string &HttpRequest::getVersion() const {
    return version;
}

void HttpRequest::setVersion(const string &version) {
    HttpRequest::version = version;
}

int HttpRequest::getIp() const {
    return ip;
}

void HttpRequest::setIp(int ip) {
    HttpRequest::ip = ip;
}

short HttpRequest::getPort() const {
    return port;
}

void HttpRequest::setPort(short port) {
    HttpRequest::port = port;
}

const unordered_map<string, string> &HttpRequest::getHeaders() const {
    return headers;
}

void HttpRequest::setHeader(const string &head, const string &val) {
    headers[head] = val;
}

ostream &operator<<(ostream &os, const HttpRequest &request) {
    os << "status: " << int(request.status) << " method: " << request.method << " url: " << request.path << " query: "
       << request.query << " version: " << request.version << " ip: " << request.ip << " port: " << request.port
       << endl;
    os << "Headers -> " << endl;
    for (auto i: request.headers) {
        os << i.first << ":" << i.second << endl;
    }

    return os;
}

char *HttpRequest::getContentPtr() const {
    return contentPtr;
}

void HttpRequest::setContentPtr(char *contentPtr) {
    HttpRequest::contentPtr = contentPtr;
}

unsigned int HttpRequest::getContentLength() const {
    return contentLength;
}

void HttpRequest::setContentLength(unsigned int contentLength) {
    HttpRequest::contentLength = contentLength;
}

unsigned int HttpRequest::getReceivedLength() const {
    return receivedLength;
}

void HttpRequest::setReceivedLength(unsigned int receivedLength) {
    HttpRequest::receivedLength = receivedLength;
}

void HttpRequest::reset() {
    status = ParseState::EXPECT_REQUEST_LINE;
    if (contentPtr != nullptr) {
        free(contentPtr);
        contentPtr = nullptr;
        receivedLength = 0;
        contentLength = 0;
    }
}
