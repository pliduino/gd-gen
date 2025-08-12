#pragma once

#include <lexer/TokenStream.h>

#include <filesystem>
#include <fstream>
#include <queue>
#include <string>

class Lexer
{
    unsigned int cur_line = 1;
    inline void ignore_until_newline();
    inline void ignore_until_end_comment();
    inline TokenValue read_identifier();
    inline TokenValue read_number();
    inline TokenValue read_string();
    std::ifstream file;
    std::string file_name;

   public:
    Lexer(std::filesystem::path path)
    {
        this->file_name = path.filename().string();
        this->file = std::ifstream(path);
    }

    ~Lexer() { this->file.close(); }

    TokenStream parse();
};