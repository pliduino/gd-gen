#include "GClass.h"

GClassOptions::GClassOptions(std::queue<TokenValue> &tokens)
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

        if (token.value == "Resource")
        {
            is_resource = true;
        }
        else if (token.value == "CustomBindings")
        {
            custom_bindings = true;
        }

        not_first = true;
    }
}

GClass::GClass(std::queue<TokenValue> &tokens)
{
    options = GClassOptions(tokens);

    TokenValue token;

    if (tokens.front().token != GToken::Class)
    {
        std::cerr << "GClass expected 'class'";
        exit(1);
    }
    tokens.pop();

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Identifier)
    {
        std::cerr << "GClass expected an identifier";
        exit(1);
    }
    name = token.value;

    if (tokens.front().token != GToken::Colon)
    {
        std::cerr << "GClass expected ':', every GClass needs to inherit something.\n";
        exit(1);
    }
    tokens.pop();

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Public)
    {
        std::cerr << "GClass expected 'public' access modifier\n";
        exit(1);
    }

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Identifier)
    {
        std::cerr << "GClass expected an identifier\n";
        exit(1);
    }
    parentName = token.value;
}
