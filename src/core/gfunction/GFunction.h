#pragma once

#include <core/general/GArgument.h>

#include <string>
#include <vector>

struct GFunctionOptions
{
    GFunctionOptions() {}

    GFunctionOptions(TokenStream &token_stream);
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
    GFunction(TokenStream &token_stream);
};
