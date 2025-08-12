#pragma once

#include <core/general/GType.h>
#include <core/gproperty/GPropertyOptions.h>
#include <lexer/lexer.h>

#include <queue>
#include <string>

struct GProperty
{
    GType variantType = GType::Invalid;
    std::string rawType;
    std::string name;
    bool isPointer = false;
    GPropertyOptions options;

    GProperty() {}
    GProperty(TokenStream &token_stream);
};