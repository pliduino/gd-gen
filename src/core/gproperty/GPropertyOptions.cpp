#include "GPropertyOptions.h"

#include <helpers/logger.h>

GPropertyOptions::GPropertyOptions(TokenStream &token_stream)
{
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("GProperty options expected left parenthesis, got " + token.value,
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
                Logger::log("GProperty options expected ',', got " + token.value, LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GProperty options expected identifier, got " + token.value,
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }

        if (token.value == "Required")
        {
            isRequired = true;
        }
        else if (token.value == "HideInInspector")
        {
            hideInInspector = true;
        }
        else if (token.value == "MultilineText")
        {
            multiline_text = true;
        }
        else if (token.value == "NoStorage")
        {
            noStorage = true;
        }
        else if (token.value == "ReadOnly")
        {
            readOnly = true;
        }
        else if (token.value == "Description")
        {
            token = token_stream.next();

            if (token.token != GToken::LeftParenthesis)
            {
                Logger::log("Description expected left parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            token = token_stream.next();

            if (token.token != GToken::String)
            {
                Logger::log("Description expected string, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            description = token.value;

            token = token_stream.next();

            if (token.token != GToken::RightParenthesis)
            {
                Logger::log("Description expected right parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
        }
        else if (token.value == "ShowIf")
        {
            token = token_stream.next();

            if (token.token != GToken::LeftParenthesis)
            {
                Logger::log("ShowIf expected left parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            std::string condition;

            bool at_least_one = false;

            token = token_stream.next();
            // Converts tokens between parenthesis to string
            while (token.token != GToken::RightParenthesis)
            {
                if (token_stream.empty())
                {
                    Logger::log("ShowIf expected right parenthesis", LogLevel::Error,
                                token_stream.get_filename(), token.line);
                    exit(1);
                }
                at_least_one = true;
                condition += token.value;
                token = token_stream.next();
            }

            if (!at_least_one)
            {
                Logger::log("ShowIf expected at least one token inside parenthesis",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            show_if = condition;
        }
        else if (token.value == "Getter")
        {
            token = token_stream.next();

            if (token.token != GToken::LeftParenthesis)
            {
                Logger::log("Getter expected left parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            token = token_stream.next();

            if (token.token != GToken::Identifier)
            {
                Logger::log("Getter expected identifier, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            custom_getter = token.value;

            token = token_stream.next();

            if (token.token != GToken::RightParenthesis)
            {
                Logger::log("Getter expected right parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
        }
        else if (token.value == "Setter")
        {
            token = token_stream.next();

            if (token.token != GToken::LeftParenthesis)
            {
                Logger::log("Getter expected left parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            token = token_stream.next();

            if (token.token != GToken::Identifier)
            {
                Logger::log("Getter expected identifier, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            custom_setter = token.value;

            token = token_stream.next();

            if (token.token != GToken::RightParenthesis)
            {
                Logger::log("Getter expected right parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
        }
        else if (token.value == "Range")
        {
            range_enabled = true;

            token = token_stream.next();

            if (token.token != GToken::LeftParenthesis)
            {
                Logger::log("Range expected left parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            token = token_stream.next();

            if (token.token != GToken::Float && token.token != GToken::Integer)
            {
                Logger::log("Range expected a number for minimum, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            range.min = std::stof(token.value);

            token = token_stream.next();

            if (token.token != GToken::Comma)
            {
                Logger::log("Range expected comma, got '" + token.value + "'", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }

            token = token_stream.next();

            if (token.token != GToken::Float && token.token != GToken::Integer)
            {
                Logger::log("Range expected a number for maximum, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            range.max = std::stof(token.value);

            token = token_stream.next();

            if (token.token == GToken::Comma)
            {
                token = token_stream.next();

                if (token.token != GToken::Float && token.token != GToken::Integer)
                {
                    Logger::log("Range expected a number for step, got '" + token.value + "'",
                                LogLevel::Error, token_stream.get_filename(), token.line);
                    exit(1);
                }

                range.step = std::stof(token.value);

                token = token_stream.next();
            }

            if (token.token != GToken::RightParenthesis)
            {
                Logger::log("Range expected right parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
        }
        else if (token.value == "Group")
        {
            token = token_stream.next();

            if (token.token != GToken::LeftParenthesis)
            {
                Logger::log("Group expected an left parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }

            token = token_stream.next();

            if (token.token != GToken::String)
            {
                Logger::log("Group expected an string, got '" + token.value + "'", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            group = token.value;

            token = token_stream.next();

            if (token.token != GToken::RightParenthesis)
            {
                Logger::log("Group expected an right parenthesis, got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
        }
        else
        {
            Logger::log("Unknown flag, got '" + token.value + "'", LogLevel::Error,
                        token_stream.get_filename(), token.line);
            exit(1);
        }

        not_first = true;
    }
}
