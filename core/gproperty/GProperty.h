#pragma once

#include <string>
#include <queue>

#include <lexer/lexer.h>
#include <core/general/GType.h>

struct GPropertyOptions
{
    struct Range
    {
        float min = 0.0;
        float max = 0.0;
        float step = 0.0;
    };

    bool isRequired = false;
    bool hideInInspector = false;
    bool noStorage = false;
    bool multiline_text = false;
    std::string description;
    std::string show_if;
    std::string custom_getter;
    std::string custom_setter;
    std::string group;

    // Maybe create a proper struct parsing stuff instead of this
    bool range_enabled = false;
    Range range;

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