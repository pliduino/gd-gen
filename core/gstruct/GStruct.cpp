#include "GStruct.h"

#include <helpers/logger.h>

GStructOptions::GStructOptions(TokenStream &token_stream)
{
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("GStruct options expected left parenthesis, got '" + token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }

    bool not_first = false;

    // Reading Options
    while (!token_stream.empty())
    {
        token = token_stream.next();

        if (token.token == GToken::RightParenthesis)
        {
            break;
        }

        if (not_first)
        {
            if (token.token != GToken::Comma)
            {
                Logger::log("GStruct options expected ',', got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GStruct options expected identifier, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }

        not_first = true;
    }
}

GStruct::GStruct(TokenStream &token_stream)
{
    options = GStructOptions(token_stream);

    TokenValue token = token_stream.next();

    if (token.token != GToken::Struct)
    {
        Logger::log("GStruct expected 'struct', got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }

    token = token_stream.next();
    if (token.token != GToken::Identifier)
    {
        Logger::log("GStruct expected an identifier, got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }
    name = token.value;

    token = token_stream.next();
    if (token.token != GToken::LeftCurlyBrace)
    {
        Logger::log("GStruct expected '{', got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }

    int level = 1;

    while (!token_stream.empty())
    {
        token = token_stream.next();

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
            properties.push_back(GProperty(token_stream));
        }
    }

    Logger::log("GStruct expected '}', got EOF", LogLevel::Error, token_stream.get_filename(),
                token.line);
    exit(1);
}
