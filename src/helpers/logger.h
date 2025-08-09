#pragma once

#include <iostream>
#include <string>

enum LogLevel
{
    Info,
    Warning,
    Error
};

namespace Logger
{
void log(std::string error, LogLevel level, std::string file, int line_number);
}  // namespace Logger
