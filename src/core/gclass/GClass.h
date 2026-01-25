#pragma once

#include <core/gfunction/GFunction.h>
#include <core/gproperty/GProperty.h>
#include <core/gsignal/GSignal.h>
#include <lexer/lexer.h>

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
    bool is_node = false;
    bool custom_bindings = false;
    GClassType class_type = GClassType::Base;

    GClassOptions() {}

    GClassOptions(TokenStream &token_stream);
};

struct GClass
{
    std::string name;
    std::string parentName;
    std::filesystem::path path;
    std::vector<GSignal> signals;
    std::vector<GProperty> properties;
    std::vector<GFunction> functions;
    bool has_ready_override;
    unsigned int generator_line;
    GClassOptions options;

    GClass() {}
    GClass(TokenStream &token_stream);
};
