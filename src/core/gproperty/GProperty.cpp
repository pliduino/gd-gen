#include "GProperty.h"

#include <helpers/logger.h>

GProperty::GProperty(TokenStream &token_stream)
{
    options = GPropertyOptions(token_stream);
    TokenValue token;

    token = token_stream.next();
    if (token.token != GToken::Identifier)
    {
        Logger::log("GProperty expected an identifier for type, got " + token.value,
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }
    rawType.append(token.value);

    token = token_stream.next();
    if (token.token == GToken::Asterisk)
    {
        variantType = GType::NodePathToRaw;
        token = token_stream.next();
    }
    else if (rawType == "float" || rawType == "double")
    {
        variantType = GType::Float;
    }
    else if (rawType == "int")
    {
        variantType = GType::Int;
    }
    else if (rawType == "bool")
    {
        variantType = GType::Boolean;
    }
    else if (rawType == "String")
    {
        variantType = GType::String;
    }
    else if (rawType == "PackedByteArray")
    {
        variantType = GType::PackedByteArray;
    }
    else if (rawType.starts_with("Ref<"))
    {
        variantType = GType::Resource;
    }
    else
    {
        variantType = GType::Object;
    }

    if (token.token != GToken::Identifier)
    {
        Logger::log("GProperty expected an identifier for name, got " + token.value,
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }
    name = token.value;
}
