#pragma once

#include <lexer/TokenStream.h>

#include <string>

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
    bool readOnly = false;
    // std::string editable_if;

    std::string description;
    std::string show_if;
    std::string custom_getter;
    std::string custom_setter;
    std::string group;

    // Maybe create a proper struct parsing stuff instead of this
    bool range_enabled = false;
    Range range;

    GPropertyOptions() {}

    GPropertyOptions(TokenStream &token_stream);
};
