#pragma once

#include <vector>
#include <variant>
#include <string>

#include "token.h"

namespace lexer
{

class Lexer 
{

public:
    using TokenArr_t = std::vector<Token>;

    Lexer(std::string src_buf, std::string file_name = "unknown_file");

    TokenArr_t tokenize ();
    const std::string& getFileName() const; 

private:

    std::string buf_;
    TokenArr_t token_arr_;
    std::string file_name_;

    std::size_t pos_      = 0;
    std::size_t cur_line_ = 1;
    std::size_t cur_col_  = 1;

    Token readInt();
    Token readIdent();

    bool atEnd(std::size_t pos)  const;
    char getSym(std::size_t pos) const;
    void skipSpaces();
    void movePosTo(std::size_t dst_pos);

    Token makeErrorToken(const std::string messege);
    Token makeIntToken(const int value);
    Token makeEndToken();
    Token makeIdentToken(const std::string ident);
};

}; //lexer namespace