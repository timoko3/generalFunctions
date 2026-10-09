#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "token_stream.h"

namespace lexer
{

TokenStream::TokenStream(std::vector<Token> tokens, std::string file_name)
    : tokens_(std::move(tokens)),
      file_name_(std::move(file_name))
{
    if (tokens_.empty())
    {
        throw std::invalid_argument("Get empty token list");
    }

    const Token& last = tokens_.back();

    if (last.type == TokenType::ERROR)
    {
        failAtToken(last, "Token with ERROR");
    }

    if (last.type != TokenType::END)
    {
        failAtToken(last, "Token list must end with END");
    }
}

const std::string& TokenStream::getFileName() const
{
    return file_name_;
}

const Token& TokenStream::peekToken() const
{
    return tokens_.at(pos_);
}

void TokenStream::movePos()
{
    const Token& token = peekToken();

    if (token.type != TokenType::END)
    {
        ++pos_;
    }

    return;
}

bool TokenStream::atEnd() const
{
    return peekToken().type == TokenType::END;
}

bool TokenStream::isMatchType(TokenType type)
{
    if (peekToken().type != type)
    {
        return false;
    } 
    
    return true;
}

const Token& TokenStream::expectToken(TokenType type, const std::string& msg)
{
    const Token& curToken = peekToken();

    if (curToken.type != type)
    {
        failAtToken(curToken, msg);
    }

    movePos();
    return curToken;
}

int TokenStream::expectInt(const std::string& msg)
{
    return std::get<int>(expectToken(TokenType::INT, msg).data);
}

const std::string& TokenStream::expectIdent(const std::string& msg)
{
    return std::get<std::string>(expectToken(TokenType::IDENTIFIER, msg).data);
}

void TokenStream::failAtToken(const Token& token, const std::string& msg) const
{
    throw std::runtime_error(file_name_ + ":" + std::to_string(token.line) + ":" +
                             std::to_string(token.col) + ": " + msg);
}

} // namespace lexer