#ifndef LEXER_H
#define LEXER_H

#include <variant>
#include <string>

enum TokenType
{
    INT,
    END,
    ERROR,
    IDENTIFIER,
};

struct Token
{
    TokenType type;
    std::variant<int, std::string> value;
    size_t line;
    size_t column;
};

class Lexer
{
    std::string src_file_name_;
    std::string buffer_;

    size_t pos_        = 0;
    size_t cur_line_   = 1;
    size_t cur_column_ = 1;

public:
    explicit Lexer(const std::string& src_file_name);
    ~Lexer() = default;
    Token getNextToken();
    const std::string& getSrcFileName() const;

private:
    Token getIntNum();
    Token getIdentifier();
    void  skipSpaces();
    bool  atEnd() const;
    char  curChar() const;
    void  movePos();
};

#endif /* LEXER_H */