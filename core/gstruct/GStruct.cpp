#include "GStruct.h"

GStructOptions::GStructOptions(std::queue<TokenValue> &tokens)
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

GStruct::GStruct(std::queue<TokenValue> &tokens)
{
    options = GStructOptions(tokens);

    TokenValue token;

    if (tokens.front().token != GToken::Struct)
    {
        std::cerr << "GStruct expected 'struct'";
        exit(1);
    }
    tokens.pop();

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Identifier)
    {
        std::cerr << "GStruct expected an identifier";
        exit(1);
    }
    name = token.value;

    if (tokens.front().token != GToken::LeftCurlyBrace)
    {
        std::cerr << "GStruct expected '{'";
        exit(1);
    }
    tokens.pop();

    int level = 1;

    while (!tokens.empty())
    {
        token = tokens.front();
        tokens.pop();

        if (token.token == GToken::LeftCurlyBrace)
        {
            level++;
        }
        else if (token.token == GToken::RightCurlyBrace)
        {
            level--;
            if (level == 0)
            {
                return;
            }
        }
        else if (token.token == GToken::GPROPERTY)
        {
            properties.push_back(GProperty(tokens));
        }
    }

    std::cerr << "GStruct expected '}'";
    exit(1);
}
