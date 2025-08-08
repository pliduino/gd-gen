#pragma once

#include <string>
#include <vector>
#include <queue>
#include <lexer/lexer.h>
#include <core/general/GType.h>

struct GArgument
{
    GType variantType = GType::Invalid;
    std::string raw_type;
    std::string name;
    bool isPointer = false;

    static std::vector<GArgument> read_garguments(std::queue<TokenValue> &tokens);
};