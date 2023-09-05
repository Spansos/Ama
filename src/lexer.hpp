#pragma once

#include <vector>
#include "token.hpp"


class Lexer
{
public:
    Lexer (
        std::vector<TokenType> tokenTypes
    );
    std::vector<Token> lex (
        const std::string & code
    ) const;
private:
    std::vector<TokenType> _tokenTypes;
};