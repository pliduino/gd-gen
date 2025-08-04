#include "GArgument.h"

#include <iostream>

std::vector<GArgument> GArgument::read_garguments(std::queue<TokenValue> &tokens)
{
    std::vector<GArgument> arguments;
    TokenValue token;

    if (tokens.front().token != GToken::LeftParenthesis)
    {
        std::cerr << "GArguments expected left parenthesis got '" << tokens.front().value << "'\n";
        exit(1);
    }
    tokens.pop();

    while (!tokens.empty())
    {
        token = tokens.front();
        tokens.pop();

        if (token.token == GToken::RightParenthesis)
        {
            break;
        }

        GArgument gArgument = {};

        if (arguments.size() > 0)
        {
            if (token.token != GToken::Comma)
            {
                std::cerr << "GArguments expected ','\n";
                exit(1);
            }
            token = tokens.front();
            tokens.pop();
        }

        if (token.token != GToken::Identifier)
        {
            std::cerr << "GArguments expected identifier after comma, got '" << token.value << "'\n";
            exit(1);
        }
        gArgument.raw_type = token.value;

        // TODO: This is copied from GPROPERTY, create a common function
        token = tokens.front();
        if (token.token == GToken::Asterisk)
        {
            gArgument.variantType = GType::NodePathToRaw;
            tokens.pop();
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

        token = tokens.front();
        tokens.pop();
        if (token.token != GToken::Identifier)
        {
            std::cerr << "GArguments expected identifier after type, got '" << token.value << "'\n";
            exit(1);
        }
        gArgument.name = token.value;

        arguments.push_back(gArgument);
    }

    return arguments;
}