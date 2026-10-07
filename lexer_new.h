#pragma once

#include <vector>
#include <variant>
#include <string>

namespace lexer
{

enum class TokenType
{
    INT,
    END,
    ERROR,
    IDENTIFIER
};

struct Token
{
    TokenType type;
    std::variant<int, std::string> data;

    std::size_t line;
    std::size_t col;
};

class Lexer 
{

public:
    using TokenArr_t = std::vector<Token>;

    Lexer(const std::string& src_buf);
    TokenArr_t run ();

private:

    const std::string& buf_;
    TokenArr_t token_arr_;

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