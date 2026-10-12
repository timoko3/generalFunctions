#include "token_stream.h"

#include <stdexcept>
#include <utility>
#include <variant>

namespace lexer {

TokenStream::TokenStream(std::string file_name)
    : file_name_(std::move(file_name)) {}

void TokenStream::verify() const {
    if (tokens_.empty()) {
        throw std::invalid_argument("Token list is empty");
    }

    const Token& last = tokens_.back();

    if (last.type == TokenType::ERROR) {
        failAtToken(last, std::get<std::string>(last.data));
    }

    if (last.type != TokenType::END) {
        failAtToken(last, "Token list must end with END");
    }
}

const std::string& TokenStream::getFileName() const {
    return file_name_;
}

const Token& TokenStream::peekToken() const {
    return tokens_.at(pos_);
}

void TokenStream::movePos() {
    if (peekToken().type != TokenType::END) {
        ++pos_;
    }
}

void TokenStream::addToken(Token token) {
    tokens_.push_back(std::move(token));
}

bool TokenStream::atEnd() const {
    return peekToken().type == TokenType::END;
}

bool TokenStream::isMatchType(TokenType type) const {
    return peekToken().type == type;
}

const Token& TokenStream::expectToken(TokenType type, const std::string& msg) {
    const Token& curToken = peekToken();
    if (curToken.type != type) {
        failAtToken(curToken, msg);
    }

    movePos();
    return curToken;
}

int TokenStream::expectInt(const std::string& msg) {
    return std::get<int>(expectToken(TokenType::INT, msg).data);
}

const std::string& TokenStream::expectIdent(const std::string& msg) {
    return std::get<std::string>(expectToken(TokenType::IDENTIFIER, msg).data);
}

void TokenStream::failAtToken(const Token& token, const std::string& msg) const {
    throw std::runtime_error(file_name_ + ":" + std::to_string(token.line) + ":" +
                             std::to_string(token.col) + ": " + msg);
}

} // namespace lexer
