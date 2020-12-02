//
// Created by cheng on 2020/12/2.
//

#include "Logger.h"

FILE *Logger::logPtr{nullptr};
Logger::Level Logger::level{Logger::Level::INFO};
char Logger::levelMap[4][8] = {"DEBUG", "INFO", "WARNING", "ERROR"};
