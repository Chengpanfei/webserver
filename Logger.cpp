//
// Created by cheng on 2020/12/2.
//

#include "Logger.h"

FILE *Logger::logPtr{nullptr};
Logger::Level Logger::level{Logger::Level::INFO};
char Logger::levelMap[4][8] = {"DEBUG", "INFO", "WARNING", "ERROR"};

void Logger::init(const char *logFileName, Logger::Level logLevel) {
    logPtr = fopen(logFileName, "a");
    level = logLevel;
}

void Logger::info(const char *fmt, ...) {
    if (Level::INFO < level) return;
    char msg[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    log(Level::INFO, msg);
}

void Logger::log(Logger::Level logLevel, const char *msg) {
    time_t now = time(nullptr);
    char buf[128];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(logPtr, "%s %s: ", buf, levelMap[int(logLevel)]);
    fprintf(logPtr, "%s\n", msg);
    fflush(logPtr);
}

void Logger::debug(const char *fmt, ...) {
    if (Level::DEBUG < level) return;
    char msg[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    log(Level::DEBUG, msg);
}

void Logger::warning(const char *fmt, ...) {
    if (Level::WARNING < level) return;
    char msg[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    log(Level::WARNING, msg);
}

void Logger::error(const char *fmt, ...) {
    if (Level::ERROR < level) return;
    char msg[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    log(Level::ERROR, msg);
}
