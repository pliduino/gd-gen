#pragma once

#include <iostream>

#include <parser/parser.h>
#include <core/gproperty/GProperty.h>
#include <core/general/GArgument.h>

struct GSignalOptions
{
    GSignalOptions() {}

    GSignalOptions(std::queue<TokenValue> &tokens);
};

struct GSignal
{
    std::string name;
    std::vector<GArgument> arguments;
    GSignalOptions options;

    GSignal() {}
    GSignal(std::queue<TokenValue> &tokens);
};