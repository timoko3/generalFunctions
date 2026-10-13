#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "token.h"

namespace lexer {

class Lexer;

class TokenStream {
    friend class Lexer;

public:
    TokenStream(std::string file_name = "unknown_file");

    const std::string& getFileName() const;

    const Token& peekToken() const;
    void movePos();

    bool atEnd() const;
    bool isMatchType(TokenType type) const;

    const Token& expectToken(TokenType type, const std::string& msg);
    int expectInt(const std::string& msg);
    const std::string& expectIdent(const std::string& msg);

    void failAtToken(const Token& token, const std::string& msg) const;
    void verify() const;

private:
    void addToken(Token token);

    std::vector<Token> tokens_;
    std::string fileName_;
    std::size_t pos_ = 0;
};

} // namespace lexer