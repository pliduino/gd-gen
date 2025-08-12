#pragma once

#include <queue>
#include <string>

enum class GToken
{
    Invalid = 0,
    GFUNCTION,
    Struct,
    GPROPERTY,
    GCLASS,
    GSTRUCT,
    GENUM,
    GSIGNAL,
    Class,
    Identifier,
    Integer,
    Float,
    Equal,
    Const,
    Pointer,
    LeftParenthesis,
    RightParenthesis,
    LeftCurlyBrace,
    RightCurlyBrace,
    Colon,
    String,
    Asterisk,
    Comma,
    GENERATED_BODY,
    Public,
    Enum,
    Private,
    Protected,
    Inline,
    Static,
    Virtual
};

struct TokenValue
{
    GToken token;
    std::string value;
    unsigned int line;
};

class TokenStream
{
    std::queue<TokenValue> tokens;
    std::string filename;

   public:
    TokenStream(std::queue<TokenValue> tokens, std::string filename)
    {
        this->tokens = tokens;
        this->filename = filename;
    }

    TokenValue next()
    {
        TokenValue token = tokens.front();
        tokens.pop();
        return token;
    }

    bool empty() { return tokens.empty(); }

    std::string get_filename() { return this->filename; }
};