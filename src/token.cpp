#include "token.hpp"

Token::Token() :
    start(-1),
    type((TokenType)-1)
{ }


Token::Token (
    TokenType type,
    size_t start
) :
    start(start),
    type(type)
{ }