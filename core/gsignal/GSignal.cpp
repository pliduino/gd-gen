#include "GSignal.h"

GSignalOptions::GSignalOptions(std::queue<TokenValue> &tokens)
{
    TokenValue token;
    bool not_first = false;

    // Reading Options
    while (!tokens.empty())
    {
        token = tokens.front();
        tokens.pop();

        if (token.token == GToken::RightParenthesis)
        {
            if (!not_first)
            {
                std::cerr << "Expect at least one option due to comma\n";
                exit(1);
            }
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

GSignal::GSignal(std::queue<TokenValue> &tokens)
{

    TokenValue token;

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::LeftParenthesis)
    {
        std::cerr << "GSignal expected left parenthesis, got '" << token.value << "'\n";
        exit(1);
    }

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Identifier)
    {
        std::cerr << "GSignal expected an identifier for name, got '" << token.value << "'\n";
        exit(1);
    }
    name = token.value;

    arguments = GArgument::read_garguments(tokens);

    token = tokens.front();
    tokens.pop();

    if (token.token == GToken::RightParenthesis)
    {
        return;
    }

    if (token.token != GToken::Colon)
    {
        std::cerr << "GSignal expected ','";
        exit(1);
    }

    options = GSignalOptions(tokens);
}
