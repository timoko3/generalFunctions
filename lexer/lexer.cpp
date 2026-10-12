#include <vector>
#include <string>
#include <cctype>
#include <limits>

#include "lexer.h"

namespace lexer
{

namespace
{
    bool isIdentSym(const unsigned char c)
    {
        return std::isdigit(c) || std::isalpha(c);
    }

    bool willOverflow(const int value, const int digit)
    {
        return value > (std::numeric_limits<int>::max() - digit) / 10;
    }
}

const std::string& Lexer::getFileName() const
{
    return fileName_;
}

Lexer::Lexer(std::string src_buf, std::string file_name) 
    : buf_(std::move(src_buf)), fileName_(std::move(file_name)) {};

bool Lexer::atEnd(std::size_t pos) const {
    return pos >= buf_.size();
}

void Lexer::skipSpaces() {
    while (!atEnd(pos_) && std::isspace(getSym(pos_)))
        advanceTo(pos_ + 1);
}

unsigned char Lexer::getSym(std::size_t pos) const {
    return static_cast<unsigned char> (buf_.at(pos));
}

void Lexer::advanceTo(std::size_t dst_pos) {
    while(pos_ < dst_pos) {
        if (atEnd(pos_)) return;

        if (getSym(pos_) == '\n') {
            ++curLine_;
            curCol_ = 1;
        }

        else {
            ++curCol_;
        }

        ++pos_;
    }
}

Token Lexer::readInt()
{
    std::size_t pos = pos_;

    int value = 0;

    if (atEnd(pos) || !std::isdigit((getSym(pos)))) {
        return makeErrorToken("It is not INT");
    }

    while (!atEnd(pos) && std::isdigit((getSym(pos)))) {
        const int digit = getSym(pos) - '0';

        if (willOverflow(value, digit)) {
            return makeErrorToken("INT is overflow");
        }

        value = value * 10 + digit;
        pos++;
    }

    if (!atEnd(pos) && !std::isspace(getSym(pos))) {
        return makeErrorToken("It is not INT");
    }

    Token retToken = makeIntToken(value);
    advanceTo(pos);

    return retToken;
}

Token Lexer::readIdent() {
    std::size_t pos = pos_;

    while (!atEnd(pos) && isIdentSym(getSym(pos))) {
        ++pos;
    }

    Token retToken = makeIdentToken(buf_.substr(pos_, pos - pos_));
    advanceTo(pos);

    return retToken;
}

Token Lexer::makeErrorToken(std::string messege) const {
    return {TokenType::ERROR, std::move(messege), curLine_, curCol_};
}

Token Lexer::makeIntToken(const int value) const {
    return {TokenType::INT, value, curLine_, curCol_};
}

Token Lexer::makeEndToken() const {
    return {TokenType::END, 0, curLine_, curCol_};
}

Token Lexer::makeIdentToken(std::string ident) const {
    return {TokenType::IDENTIFIER, std::move(ident), curLine_, curCol_};
}

Lexer::TokenArr_t Lexer::tokenize () {
    while (true) {
        skipSpaces();

        if (atEnd(pos_))
            break;

        Token cur_token = readInt();

        if (cur_token.type != TokenType::ERROR) {
            tokenArr_.push_back(cur_token);
            continue;
        }

        if (isIdentSym(getSym(pos_))) {
            tokenArr_.push_back(readIdent());
        }

        if (!atEnd(pos_) && !std::isspace(getSym(pos_))) {
            tokenArr_.push_back(makeErrorToken("Unknown sym"));
            return tokenArr_;
        }
    }
    
    tokenArr_.push_back(makeEndToken());
    return tokenArr_;
}


} //lexer namespace