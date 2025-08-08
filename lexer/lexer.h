

#pragma once

#include <string>
#include <queue>
#include <fstream>
#include <filesystem>

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

std::string GTokenToString(GToken token);

struct TokenValue
{
    GToken token;
    std::string value;
    unsigned int line;
};

class Parser
{
    unsigned int cur_line = 1;
    inline void ignore_until_newline();
    inline void ignore_until_end_comment();
    inline TokenValue read_identifier();
    inline TokenValue read_number();
    inline TokenValue read_string();
    std::ifstream file;

public:
    Parser(std::filesystem::path path)
    {
        this->file = std::ifstream(path);
    }

    ~Parser()
    {
        this->file.close();
    }

    std::queue<TokenValue> parse();
};