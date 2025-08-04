#include "GProperty.h"

#include <iostream>

GProperty::GProperty(std::queue<TokenValue> &tokens)
{
    options = GPropertyOptions(tokens);
    TokenValue token;

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Identifier)
    {
        std::cerr << "GProperty expected an identifier, got " << token.value << '\n';
        exit(1);
    }
    rawType.append(token.value);

    token = tokens.front();
    if (token.token == GToken::Asterisk)
    {
        variantType = GType::NodePathToRaw;
        tokens.pop();
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

    token = tokens.front();
    tokens.pop();
    if (token.token != GToken::Identifier)
    {
        std::cerr << "GProperty expected an identifier, got " << token.value << " raw type " << rawType << '\n';
        exit(1);
    }
    name = token.value;
}

GPropertyOptions::GPropertyOptions(std::queue<TokenValue> &tokens)
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
        else if (token.value == "Description")
        {
            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::LeftParenthesis)
            {
                std::cerr << "Description expected left parenthesis, got '" << token.value << "'\n";
                exit(1);
            }

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::String)
            {
                std::cerr << "Description expected string, got '" << token.value << "'\n";
                exit(1);
            }

            description = token.value;

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::RightParenthesis)
            {
                std::cerr << "Description expected right parenthesis, got '" << token.value << "'\n";
                exit(1);
            }
        }
        else if (token.value == "ShowIf")
        {

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::LeftParenthesis)
            {
                std::cerr << "ShowIf expected left parenthesis, got '" << token.value << "'\n";
                exit(1);
            }

            std::string condition;

            bool at_least_one = false;

            // Converts tokens between parenthesis to string
            while (tokens.front().token != GToken::RightParenthesis)
            {
                at_least_one = true;
                condition += tokens.front().value;
                tokens.pop();
                if (tokens.empty())
                {
                    std::cerr << "ShowIf expected right parenthesis" << "'\n";
                    exit(1);
                }
            }

            if (!at_least_one)
            {
                std::cerr << "ShowIf expected at least one token inside parenthesis" << "'\n";
                exit(1);
            }

            show_if = condition;

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::RightParenthesis)
            {
                std::cerr << "ShowIf expected right parenthesis, got '" << token.value << "'\n";
                exit(1);
            }
        }
        else if (token.value == "Getter")
        {
            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::LeftParenthesis)
            {
                std::cerr << "Getter expected left parenthesis, got '" << token.value << "'\n";
                exit(1);
            }

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::Identifier)
            {
                std::cerr << "Getter expected identifier, got '" << token.value << "'\n";
                exit(1);
            }

            custom_getter = token.value;

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::RightParenthesis)
            {
                std::cerr << "Getter expected right parenthesis, got '" << token.value << "'\n";
                exit(1);
            }
        }
        else if (token.value == "Setter")
        {
            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::LeftParenthesis)
            {
                std::cerr << "Getter expected left parenthesis, got '" << token.value << "'\n";
                exit(1);
            }

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::Identifier)
            {
                std::cerr << "Getter expected identifier, got '" << token.value << "'\n";
                exit(1);
            }

            custom_setter = token.value;

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::RightParenthesis)
            {
                std::cerr << "Getter expected right parenthesis, got '" << token.value << "'\n";
                exit(1);
            }
        }
        else if (token.value == "HintRange")
        {
            hint_range_enabled = true;

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::LeftParenthesis)
            {
                std::cerr << "HintRange expected left parenthesis, got '" << token.value << "'\n";
                exit(1);
            }

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::Float && token.token != GToken::Integer)
            {
                std::cerr << "HintRange expected a number for minimum, got '" << token.value << "'\n";
                exit(1);
            }

            hint_range.min = std::stof(token.value);

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::Comma)
            {
                std::cerr << "HintRange expected comma, got '" << token.value << "'\n";
                exit(1);
            }

            token = tokens.front();
            tokens.pop();

            if (token.token != GToken::Float && token.token != GToken::Integer)
            {
                std::cerr << "HintRange expected a number for maximum, got '" << token.value << "'\n";
                exit(1);
            }

            hint_range.max = std::stof(token.value);

            token = tokens.front();
            tokens.pop();

            if (token.token == GToken::Comma)
            {
                token = tokens.front();
                tokens.pop();

                if (token.token != GToken::Float && token.token != GToken::Integer)
                {
                    std::cerr << "HintRange expected a number for step, got '" << token.value << "'\n";
                    exit(1);
                }

                hint_range.step = std::stof(token.value);

                token = tokens.front();
                tokens.pop();
            }

            if (token.token != GToken::RightParenthesis)
            {
                std::cerr << "HintRange expected right parenthesis, got '" << token.value << "'\n";
                exit(1);
            }
        }
        else
        {
            std::cerr << "Unknown flag, got '" << token.value << "'\n";
            exit(1);
        }

        not_first = true;
    }
}
