//
// Created by cheng on 2020/11/23.
//

#ifndef UNTITLED_HTTPREQUEST_H
#define UNTITLED_HTTPREQUEST_H

#include <string>
#include <unordered_map>
#include <ostream>
#include "../Message.h"

using namespace std;

enum class ParseState{EXPECT_REQUEST_LINE, PARSING_HEADER};


class HttpRequest: public Message {
private:
     ParseState status;
     string method; //
     string path;
     string query;
     string version;

     int ip;
     short port;

    unordered_map<string, string> headers;

public:
    HttpRequest();

    ParseState getStatus() const;

    void setStatus(const ParseState &status);

    const string &getMethod() const;

    void setMethod(const string &method);

    const string &getPath() const;

    void setPath(const string &path);

    const string &getQuery() const;

    void setQuery(const string &query);

    const string &getVersion() const;

    void setVersion(const string &version);

    int getIp() const;

    void setIp(int ip);

    short getPort() const;

    void setPort(short port);

    const unordered_map<string, string> &getHeaders() const;

    void setHeader(const string &head, const string &val);

    friend ostream &operator<<(ostream &os, const HttpRequest &request);
};


#endif //UNTITLED_HTTPREQUEST_H
