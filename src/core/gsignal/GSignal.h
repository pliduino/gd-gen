#pragma once

#include <core/general/GArgument.h>
#include <core/gproperty/GProperty.h>

#include <iostream>

struct GSignalOptions
{
    GSignalOptions() {}

    GSignalOptions(TokenStream &token_stream);
};

struct GSignal
{
    std::string name;
    std::vector<GArgument> arguments;
    GSignalOptions options;

    GSignal() {}
    GSignal(TokenStream &token_stream);
};