#pragma once

#include <iostream>

#include <lexer/lexer.h>
#include <core/gproperty/GProperty.h>
#include <core/gsignal/GSignal.h>
#include <core/gfunction/GFunction.h>

enum class GClassType
{
    Base,
    Virtual,
    Abstract,
    Runtime,
    Internal
};

std::string get_class_type_string(GClassType type);

struct GClassOptions
{
    bool is_resource = false;
    bool custom_bindings = false;
    GClassType class_type = GClassType::Base;

    GClassOptions() {}

    GClassOptions(std::queue<TokenValue> &tokens);
};

struct GClass
{
    std::string name;
    std::string parentName;
    std::filesystem::path path;
    std::vector<GSignal> signals;
    std::vector<GProperty> properties;
    std::vector<GFunction> functions;
    unsigned int generator_line;
    GClassOptions options;

    GClass() {}
    GClass(std::queue<TokenValue> &tokens);
};