#pragma once

#include <string>
#include <queue>

#include <parser/parser.h>
#include <core/general/GType.h>

struct GPropertyOptions
{
    bool isRequired = false;
    bool hideInInspector = false;
    bool noStorage = false;
    bool multiline_text = false;
    std::string description;
    std::string show_if;

    GPropertyOptions() {}

    GPropertyOptions(std::queue<TokenValue> &tokens);
};

struct GProperty
{
    GType variantType = GType::Invalid;
    std::string rawType;
    std::string name;
    bool isPointer = false;
    GPropertyOptions options;

    GProperty() {}
    GProperty(std::queue<TokenValue> &tokens);
};