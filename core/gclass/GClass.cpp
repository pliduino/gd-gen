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
        else if (token.value == "Virtual")
        {
            if (class_type != GClassType::Base)
            {
                std::cerr << "Class can't be " << get_class_type_string(GClassType::Virtual) << " and "
                          << get_class_type_string(class_type) << " at the same time\n";
                exit(1);
            }
            class_type = GClassType::Virtual;
        }
        else if (token.value == "Abstract")
        {
            if (class_type != GClassType::Base)
            {
                std::cerr << "Class can't be " << get_class_type_string(GClassType::Abstract) << " and "
                          << get_class_type_string(class_type) << " at the same time\n";
                exit(1);
            }
            class_type = GClassType::Abstract;
        }
        else if (token.value == "Runtime")
        {
            if (class_type != GClassType::Base)
            {
                std::cerr << "Class can't be " << get_class_type_string(GClassType::Runtime) << " and "
                          << get_class_type_string(class_type) << " at the same time\n";
                exit(1);
            }
            class_type = GClassType::Runtime;
        }
        else if (token.value == "Internal")
        {
            if (class_type != GClassType::Base)
            {
                std::cerr << "Class can't be " << get_class_type_string(GClassType::Internal) << " and "
                          << get_class_type_string(class_type) << " at the same time\n";
                exit(1);
            }
            class_type = GClassType::Internal;
        }
        else
        {
            std::cerr << "Unknown GCLASS option '" << token.value << "'\n";
            exit(1);
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

std::string get_class_type_string(GClassType type)
{
    switch (type)
    {
    case GClassType::Base:
        return "Base";
    case GClassType::Virtual:
        return "Virtual";
    case GClassType::Abstract:
        return "Abstract";
    case GClassType::Runtime:
        return "Runtime";
    }
    return "";
}
