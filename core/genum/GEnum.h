#pragma once

#include <lexer/lexer.h>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

struct GEnumOptions
{
    GEnumOptions() {}
    GEnumOptions(TokenStream &token_stream);
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

    GEnum(TokenStream &token_stream);
};