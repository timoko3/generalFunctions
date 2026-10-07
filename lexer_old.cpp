#include <string>
#include <sstream>
#include <cctype>
#include <limits>
#include <utility>

#include "lexer_old.h"
#include "generalFunctions/file.h"

namespace 
{
    bool isDigit(char c)
    {
        return std::isdigit(static_cast<unsigned char>(c));
    }

    bool isSpace(char c)
    {
        return std::isspace(static_cast<unsigned char>(c));
    }

    bool isAlpha(char c)
    {
        return std::isalpha(static_cast<unsigned char>(c));
    }

    bool isIdentifierSym(char c)
    {
        return isAlpha(c) || isDigit(c);
    }
}


Lexer::Lexer(const std::string& src_file_name)
    : src_file_name_(src_file_name),
      buffer_(generalFunctions::readFile(src_file_name))
{
}

bool Lexer::atEnd() const
{
    return pos_ >= buffer_.size();
}

char Lexer::curChar() const
{
    return buffer_.at(pos_);
}

void Lexer::movePos()
{
    if (atEnd()) return;

    if (curChar() == '\n')
    {
        ++cur_line_;
        cur_column_ = 1;
    }

    else
    {
        ++cur_column_;
    }

    ++pos_;
}

void Lexer::skipSpaces()
{
    while (!atEnd() && isSpace(curChar()))
        movePos();
}

const std::string& Lexer::getSrcFileName() const
{
    return src_file_name_;
}

Token Lexer::getIntNum()
{
    const std::size_t old_pos    = pos_;
    const std::size_t old_line   = cur_line_;
    const std::size_t old_column = cur_column_;

    int value = 0;
    bool overflow = false;

    if (atEnd() || !isDigit(curChar()))
    {
        return Token
        {
            TokenType::ERROR,
            std::string{"Not INT"},
            cur_line_,
            cur_column_
        };
    }

     while (!atEnd() && isDigit(curChar()))
    {
        const int digit = curChar() - '0';

        if (value > (std::numeric_limits<int>::max() - digit) / 10)
        {
            overflow = true;
        }

        else if (!overflow)
        {
            value = value * 10 + digit;
        }

        movePos();
    }

    if (!atEnd() && !isSpace(curChar()))
    {
        pos_        = old_pos;
        cur_line_   = old_line;
        cur_column_ = old_column;

        return Token
        {
            TokenType::ERROR,
            0,
            old_line,
            old_column
        };
    }

    if (overflow)
    {
        return Token 
        {
            TokenType::ERROR,
            std::string{"overflow INT value"},
            cur_line_,
            cur_column_
        };
    }

    return Token
    {
        TokenType::INT,
        value,
        old_line,
        old_column
    };
}

Token Lexer::getIdentifier()
{
    std::string value;

    while (!atEnd() && isIdentifierSym(curChar()))
    {
        value.push_back(curChar());
        movePos();
    }

    return Token
    {
        TokenType::IDENTIFIER,
        std::move(value),
        cur_line_,
        cur_column_
    };
}

Token Lexer::getNextToken()
{
    skipSpaces();

    if (atEnd())
    {
        return Token
        {
            TokenType::END,
            0,
            cur_line_,
            cur_column_
        };
    }

    Token token = getIntNum();

    if (token.type != TokenType::ERROR)
        return token;

    return getIdentifier();
}