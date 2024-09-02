#include "token.hpp"

Token::Token (
    TokenType type,
    size_t start
) :
    start(start),
    type(type)
{ }