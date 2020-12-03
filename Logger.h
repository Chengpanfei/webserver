//
// Created by cheng on 2020/12/2.
//

#ifndef WEBSERVER_LOGGER_H
#define WEBSERVER_LOGGER_H

#include <string>
#include <cstdarg>

using namespace std;

class Logger {
public:
    enum class Level {
        DEBUG = 0, INFO, WARNING, ERROR
    };

private:
    static FILE *logPtr;
    static Level level;
    static char levelMap[4][8];
public:
    static void init(const char *logFileName, Level logLevel);

    static void info(const char *fmt, ...);

    static void log(Level logLevel, const char *msg);

    static void debug(const char *fmt, ...);

    static void warning(const char *fmt, ...);

    static void error(const char *fmt, ...);
};


#endif //WEBSERVER_LOGGER_H
