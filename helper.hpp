#pragma once

#include <string>
#include <iostream>

enum LogLevel
{
    Info,
    Warning,
    Error
};

void log(std::string error, LogLevel level, std::string file, int line_number)
{
    constexpr const char *RED = "\033[31m";
    constexpr const char *GREEN = "\033[32m";
    constexpr const char *YELLOW = "\033[33m";
    constexpr const char *RESET = "\033[0m";

    switch (level)
    {
    case LogLevel::Info:
        std::cerr << GREEN << "[Info] ";
        return;
    case LogLevel::Warning:
        std::cerr << YELLOW << "[Warning] ";
        return;
    case LogLevel::Error:
        std::cerr << RED << "[Error] ";
        return;
    default:
        break;
    }

    std::cerr << file << ":" << line_number << " " << error << RESET << '\n';
}