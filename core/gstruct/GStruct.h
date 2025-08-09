#pragma once

#include <core/gproperty/GProperty.h>

struct GStructOptions
{
    GStructOptions() {}
    GStructOptions(TokenStream &token_stream);
};

struct GStruct
{
    std::string name;
    std::vector<GProperty> properties;
    GStructOptions options;

    GStruct() {}
    GStruct(TokenStream &token_stream);
};
