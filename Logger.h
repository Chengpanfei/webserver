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
    static void init(const char *logFileName, Level logLevel) {
        logPtr = fopen(logFileName, "a");
        level = logLevel;
    }

    static void info(const char *fmt, ...) {
        if (Level::INFO < level) return;
        char msg[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg, sizeof(msg), fmt, args);
        va_end(args);
        log(Level::INFO, msg);
    }

    static void log(Level logLevel, const char *msg) {
        time_t now = time(nullptr);
        char buf[128];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

        fprintf(logPtr, "%s %s: ", buf, levelMap[int(logLevel)]);
        fprintf(logPtr, "%s\n", msg);
        fflush(logPtr);
    }

    static void debug(const char *fmt, ...) {
        if (Level::DEBUG < level) return;
        char msg[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg, sizeof(msg), fmt, args);
        va_end(args);
        log(Level::DEBUG, msg);
    }

    static void warning(const char *fmt, ...) {
        if (Level::WARNING < level) return;
        char msg[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg, sizeof(msg), fmt, args);
        va_end(args);
        log(Level::WARNING, msg);
    }

    static void error(const char *fmt, ...) {
        if (Level::ERROR < level) return;
        char msg[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg, sizeof(msg), fmt, args);
        va_end(args);
        log(Level::ERROR, msg);
    }
};


#endif //WEBSERVER_LOGGER_H
