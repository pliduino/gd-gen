#pragma once

#include <optional>
#include <string>
#include <vector>
#include <iostream>

#include <lexer/lexer.h>

struct GEnumOptions
{
    GEnumOptions() {}
    GEnumOptions(std::queue<TokenValue> &tokens);
};

struct GEnum
{
    struct EnumValue
    {
        std::string name;
        std::optional<int> value;
    };

    std::string name;
    std::vector<EnumValue> values;

    GEnumOptions options;

    GEnum() {}

    GEnum(std::queue<TokenValue> &tokens);
};