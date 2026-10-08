#include <vector>
#include <variant>
#include <string>
#include <cctype>
#include <limits>
#include <iostream>

#include "lexer.h"

namespace lexer
{

namespace
{
    bool isDigit(unsigned char c)
    {
        return std::isdigit(c);
    }

    bool isSpace(unsigned char c)
    {
        return std::isspace(c);
    }

    bool isIdentSym(unsigned char c)
    {
        return std::isdigit(c) || std::isalpha(c);
    }
}

const std::string& Lexer::getFileName() const
{
    return file_name_;
}

Lexer::Lexer(std::string src_buf, std::string file_name) 
    : buf_(std::move(src_buf)), file_name_(std::move(file_name)){};

bool Lexer::atEnd(std::size_t pos) const
{
    return pos >= buf_.size();
}

void Lexer::skipSpaces()
{
    while (!atEnd(pos_) && isSpace(getSym(pos_)))
        movePosTo(pos_ + 1);
}

char Lexer::getSym(std::size_t pos) const
{
    return buf_.at(pos);
}

void Lexer::movePosTo(std::size_t dst_pos)
{
    while(pos_ < dst_pos)
    {
        if (atEnd(pos_)) return;

        if (getSym(pos_) == '\n')
        {
            ++cur_line_;
            cur_col_ = 1;
        }

        else
        {
            ++cur_col_;
        }

        ++pos_;
    }
}

Token Lexer::readInt()
{
    std::size_t pos = pos_;

    int value = 0;

    if (atEnd(pos) || !isDigit(getSym(pos)))
    {
        return makeErrorToken("It is not INT");
    }

    while (!atEnd(pos) && isDigit(getSym(pos)))
    {
        const int digit = getSym(pos) - '0';

        if (value > (std::numeric_limits<int>::max() - digit) / 10)
        {
            return makeErrorToken("INT is overflow");
        }

        value = value * 10 + digit;

        pos++;
    }

    if (!atEnd(pos) && !isSpace(getSym(pos)))
    {
        return makeErrorToken("It is not INT");
    }

    Token retToken = makeIntToken(value);

    movePosTo(pos);

    return retToken;
}

Token Lexer::readIdent()
{
    std::size_t pos = pos_;

    std::string value;

    while (!atEnd(pos) && isIdentSym(getSym(pos)))
    {
        value.push_back(getSym(pos));
        pos++;
    }

    Token retToken = makeIdentToken(value);

    movePosTo(pos);

    return retToken;
}

Token Lexer::makeErrorToken(const std::string messege)
{
    return {TokenType::ERROR, messege, cur_line_, cur_col_};
}

Token Lexer::makeIntToken(const int value)
{
    return {TokenType::INT, value, cur_line_, cur_col_};
}

Token Lexer::makeEndToken()
{
    return {TokenType::END, 0, cur_line_, cur_col_};
}

Token Lexer::makeIdentToken(const std::string ident)
{
    return {TokenType::IDENTIFIER, ident, cur_line_, cur_col_};
}

Lexer::TokenArr_t Lexer::tokenize ()
{
    while (true)
    {
        skipSpaces();

        if (atEnd(pos_))
        break;

        Token cur_token = readInt();

        if (cur_token.type != TokenType::ERROR)
        {
            token_arr_.push_back(cur_token);
            continue;
        }

        token_arr_.push_back(readIdent());

        if (!atEnd(pos_) && !isSpace(getSym(pos_)))
        {
            token_arr_.push_back(makeErrorToken("Unknown sym"));
            return token_arr_;
        }
    }
    
    token_arr_.push_back(makeEndToken());
    return token_arr_;
}


} //lexer namespace