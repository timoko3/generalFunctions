#include "lexer.h"

#include <cctype>
#include <limits>
#include <utility>

namespace lexer {
namespace {

bool isIdentSym(unsigned char c) {
    return std::isdigit(c) || std::isalpha(c);
}

bool willOverflow(int value, int digit) {
    return value > (std::numeric_limits<int>::max() - digit) / 10;
}

} // namespace

Lexer::Lexer(std::string src_buf, std::string file_name)
    : buf_(std::move(src_buf)),
      fileName_(std::move(file_name)) {}

const std::string& Lexer::getFileName() const {
    return fileName_;
}

bool Lexer::atEnd(std::size_t pos) const {
    return pos >= buf_.size();
}

unsigned char Lexer::getSym(std::size_t pos) const {
    return static_cast<unsigned char>(buf_.at(pos));
}

void Lexer::advanceTo(std::size_t dst_pos) {
    while (pos_ < dst_pos && !atEnd(pos_)) {
        if (getSym(pos_) == '\n') {
            ++curLine_;
            curCol_ = 1;
        } else {
            ++curCol_;
        }
        ++pos_;
    }
}

void Lexer::skipSpaces() {
    while (!atEnd(pos_) && std::isspace(getSym(pos_))) {
        advanceTo(pos_ + 1);
    }
}

Token Lexer::readInt() {
    std::size_t pos = pos_;
    int value = 0;

    if (atEnd(pos) || !std::isdigit(getSym(pos))) {
        return makeErrorToken("It is not INT");
    }

    while (!atEnd(pos) && std::isdigit(getSym(pos))) {
        const int digit = getSym(pos) - '0';
        if (willOverflow(value, digit)) {
            return makeErrorToken("INT is overflow");
        }
        value = value * 10 + digit;
        ++pos;
    }

    if (!atEnd(pos) && !std::isspace(getSym(pos))) {
        return makeErrorToken("It is not INT");
    }

    Token token = makeIntToken(value);
    advanceTo(pos);
    return token;
}

Token Lexer::readIdent() {

    std::size_t pos = pos_;
    while (!atEnd(pos) && isIdentSym(getSym(pos))) {
        ++pos;
    }

    Token token = makeIdentToken(buf_.substr(pos_, pos - pos_));
    advanceTo(pos);
    return token;
}

Token Lexer::makeErrorToken(std::string message) const {
    return {TokenType::ERROR, std::move(message), curLine_, curCol_};
}

Token Lexer::makeIntToken(int value) const {
    return {TokenType::INT, value, curLine_, curCol_};
}

Token Lexer::makeEndToken() const {
    return {TokenType::END, 0, curLine_, curCol_};
}

Token Lexer::makeIdentToken(std::string ident) const {
    return {TokenType::IDENTIFIER, std::move(ident), curLine_, curCol_};
}

TokenStream Lexer::tokenize() {
    TokenStream tokenStream(fileName_);

    while (true) {

        skipSpaces();

        if (atEnd(pos_)) {
            break;
        }

        Token token = readInt();

        if (token.type != TokenType::ERROR) {
            tokenStream.addToken(std::move(token));
            continue;
        }

        if (isIdentSym(getSym(pos_))) {
            tokenStream.addToken(readIdent());
        }

        if (!atEnd(pos_) && !std::isspace(getSym(pos_))) {
            tokenStream.addToken(makeErrorToken("Unknown sym"));
            tokenStream.verify(); // Throws on ERROR.
        }
    }

    tokenStream.addToken(makeEndToken());
    tokenStream.verify();

    return tokenStream;
}

} // namespace lexer
