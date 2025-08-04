#include "GFunction.h"

#include <iostream>

GFunctionOptions::GFunctionOptions(std::queue<TokenValue> &tokens)
{
    TokenValue token;

    if (tokens.front().token != GToken::LeftParenthesis)
    {
        std::cerr << "Options expected left parenthesis, got '" << tokens.front().value << "'\n";
        exit(1);
    }
    tokens.pop();

    bool not_first = false;

    // Reading Options
    while (!tokens.empty())
    {
        token = tokens.front();
        tokens.pop();

        if (token.token == GToken::RightParenthesis)
        {
            break;
        }

        if (not_first)
        {
            if (token.token != GToken::Comma)
            {
                std::cerr << "Options expected ','\n";
                exit(1);
            }
            token = tokens.front();
            tokens.pop();
        }

        if (token.token != GToken::Identifier)
        {
            std::cerr << "Options expected identifier, got '" << token.value << "'\n";
            exit(1);
        }

        not_first = true;
    }
}

GFunction::GFunction(std::queue<TokenValue> &tokens)
{
    options = GFunctionOptions(tokens);

    TokenValue token;

    while (true)
    {
        token = tokens.front();
        tokens.pop();

        if (token.token == GToken::Inline)
        {
            isInline = true;
        }
        else if (token.token == GToken::Const)
        {
            isConst = true;
        }
        else if (token.token == GToken::Static)
        {
            isStatic = true;
        }
        else if (token.token == GToken::Virtual)
        {
            isVirtual = true;
        }
        else if (token.token == GToken::Identifier)
        {
            returnType = token.value;
            break;
        }
        else
        {
            std::cerr << "GFunction expected an identifier";
            exit(1);
        }
    }

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Identifier)
    {
        std::cerr << "GFunction expected an identifier";
        exit(1);
    }
    name = token.value;

    std::vector<GArgument> arguments = GArgument::read_garguments(tokens);
    arguments = arguments;
}
