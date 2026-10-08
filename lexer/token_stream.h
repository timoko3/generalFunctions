#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "token.h"

namespace lexer
{

class TokenStream
{
public:
    TokenStream(std::vector<Token> tokens, std::string file_name = "unknown_file");

    const std::string& getFileName() const; 
    
    const Token& peekToken() const;
    const Token& movePos();

    bool atEnd() const;
    bool isMatchType(TokenType type);

    const Token& expectToken(TokenType type, const std::string& msg);
    int expectInt(const std::string& msg);
    const std::string& expectIdent(const std::string& msg);

    void failAtToken(const Token& token, const std::string& msg) const;

private:
    std::vector<Token> tokens_;
    std::string file_name_;
    std::size_t pos_ = 0;
};

} // namespace lexer