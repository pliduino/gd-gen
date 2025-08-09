#include "GArgument.h"

#include <helpers/logger.h>

#include <iostream>

std::vector<GArgument> GArgument::read_garguments(TokenStream &token_stream)
{
    std::vector<GArgument> arguments;
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("GArguments expected left parenthesis got '" + token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }

    while (!token_stream.empty())
    {
        token = token_stream.next();

        if (token.token == GToken::RightParenthesis)
        {
            break;
        }

        GArgument gArgument = {};

        if (arguments.size() > 0)
        {
            if (token.token != GToken::Comma)
            {
                Logger::log("GArguments expected ',' got '" + token.value + "'", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GArguments expected identifier for type, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }
        gArgument.raw_type = token.value;

        // TODO: This is copied from GPROPERTY, create a common function
        token = token_stream.next();
        if (token.token == GToken::Asterisk)
        {
            gArgument.variantType = GType::NodePathToRaw;
            token = token_stream.next();
        }
        else if (gArgument.raw_type == "float" || gArgument.raw_type == "double")
        {
            gArgument.variantType = GType::Float;
        }
        else if (gArgument.raw_type == "int")
        {
            gArgument.variantType = GType::Int;
        }
        else if (gArgument.raw_type == "bool")
        {
            gArgument.variantType = GType::Boolean;
        }
        else if (gArgument.raw_type == "String")
        {
            gArgument.variantType = GType::String;
        }
        else if (gArgument.raw_type == "PackedByteArray")
        {
            gArgument.variantType = GType::PackedByteArray;
        }
        else if (gArgument.raw_type.starts_with("Ref<"))
        {
            gArgument.variantType = GType::Resource;
        }
        else
        {
            gArgument.variantType = GType::Object;
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GArguments expected identifier after type, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }
        gArgument.name = token.value;

        arguments.push_back(gArgument);
    }

    return arguments;
}