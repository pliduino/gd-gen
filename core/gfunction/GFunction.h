#pragma once

#include <string>
#include <vector>

#include <parser/parser.h>
#include <core/general/GArgument.h>

struct GFunctionOptions
{
    GFunctionOptions() {}

    GFunctionOptions(std::queue<TokenValue> &tokens);
};

struct GFunction
{
    std::string name;
    std::vector<GArgument> arguments;
    std::string returnType;
    bool isStatic = false;
    bool isInline = false;
    bool isVirtual = false;
    bool isConst = false;

    GFunctionOptions options;

    GFunction() {}
    GFunction(std::queue<TokenValue> &tokens);
};
