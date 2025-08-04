#pragma once

#include <iostream>

#include <parser/parser.h>

#include <core/gproperty/GProperty.h>

struct GStructOptions
{
    GStructOptions() {}
    GStructOptions(std::queue<TokenValue> &tokens);
};

struct GStruct
{
    std::string name;
    std::vector<GProperty> properties;
    GStructOptions options;

    GStruct() {}
    GStruct(std::queue<TokenValue> &tokens);
};
