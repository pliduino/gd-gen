#include "GEnum.h"

GEnumOptions::GEnumOptions(std::queue<TokenValue> &tokens)
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

GEnum::GEnum(std::queue<TokenValue> &tokens)
{
    options = GEnumOptions(tokens);

    TokenValue token;

    if (tokens.front().token != GToken::Enum)
    {
        std::cerr << "GEnum expected 'enum'";
        exit(1);
    }
    tokens.pop();

    token = tokens.front();
    if (token.token == GToken::Class)
    {
        tokens.pop();
        token = tokens.front();
    }

    if (token.token != GToken::Identifier)
    {
        std::cerr << "GEnum expected an identifier\n";
        exit(1);
    }
    name = token.value;
    tokens.pop();

    if (tokens.front().token != GToken::LeftCurlyBrace)
    {
        std::cerr << "GEnum expected '{'";
        exit(1);
    }
    tokens.pop();

    while (!tokens.empty())
    {
        GEnum::EnumValue enumValue = {};
        token = tokens.front();
        tokens.pop();

        if (token.token == GToken::RightCurlyBrace)
        {
            break;
        }

        if (values.size() > 0)
        {
            if (token.token != GToken::Comma)
            {
                std::cerr << "GEnum expected ',', got " << token.value << "\n";
                exit(1);
            }
            token = tokens.front();
            tokens.pop();

            if (token.token == GToken::RightCurlyBrace)
            {
                break;
            }
        }

        if (token.token != GToken::Identifier)
        {
            std::cerr << "GEnum expected an identifier, got " << token.value << "\n";
            exit(1);
        }

        enumValue.name = token.value;
        token = tokens.front();

        if (token.token == GToken::Equal)
        {
            tokens.pop();
            token = tokens.front();

            if (token.token != GToken::Number)
            {
                std::cerr << "GEnum expected a number\n";
            }
            enumValue.value = std::stoi(token.value);
            tokens.pop();
        }

        values.push_back(enumValue);
    }
}
