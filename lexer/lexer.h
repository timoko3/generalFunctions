#pragma once

#include <vector>
#include <string>

#include "token.h"

namespace lexer
{

class Lexer 
{

public:
    using TokenArr_t = std::vector<Token>;

    Lexer(std::string src_buf, std::string file_name = "unknown_file");

    TokenArr_t tokenize ();
    const std::string& getFileName() const; 

private:

    std::string buf_;
    TokenArr_t  tokenArr_;
    std::string fileName_;

    std::size_t pos_     = 0;
    std::size_t curLine_ = 1;
    std::size_t curCol_  = 1;

    Token readInt();
    Token readIdent();

    bool atEnd           (std::size_t pos) const;
    unsigned char getSym (std::size_t pos) const;
    void advanceTo       (std::size_t dst_pos);
    void skipSpaces      ();
    

    Token makeErrorToken (std::string messege) const;
    Token makeIdentToken (std::string ident) const;
    Token makeIntToken   (const int value) const;
    Token makeEndToken   () const;
    
};

}; //lexer namespace