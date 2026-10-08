#pragma once

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

} //namespace lexer