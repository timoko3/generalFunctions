#pragma once

#include <cstddef>
#include <string>
#include <variant>

namespace lexer {

enum class TokenType {
    INT,
    END,
    ERROR,
    IDENTIFIER,
    DOUBLE
};

struct Token {
    TokenType type;
    std::variant<int, std::string, double> data;
    std::size_t line;
    std::size_t col;
};

} // namespace lexer
