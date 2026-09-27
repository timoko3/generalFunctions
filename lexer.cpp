#include <string>
#include <sstream>
#include <cctype>
#include <limits>
#include <utility>

#include "lexer.h"
#include "generalFunctions/file.h"

static bool isDigit(char c)
{
    return std::isdigit(static_cast<unsigned char>(c));
}

static bool isSpace(char c)
{
    return std::isspace(static_cast<unsigned char>(c));
}

static bool isAlpha(char c)
{
    return std::isalpha(static_cast<unsigned char>(c));
}

static bool isIdentifierStart(char c)
{
    return isAlpha(c);
}

static bool isIdentifierContinue(char c)
{
    return isIdentifierStart(c) || isDigit(c);
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
    int value = 0;
    bool overflow = false;

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
        cur_line_,
        cur_column_
    };
}

Token Lexer::getIdentifier()
{
    std::string value;

    while (!atEnd() && isIdentifierContinue(curChar()))
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

    char c = curChar();

    if (isDigit(c))
        return getIntNum();

    if (isIdentifierStart(c))
        return getIdentifier();

    movePos();

    return Token
    {
        TokenType::ERROR,
        std::string{"Unknown sym"},
        cur_line_,
        cur_column_
    };
}