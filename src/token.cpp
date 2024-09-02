#include "token.hpp"

Token::Token (
    TokenType type,
    int start
) :
    type(type),
    start(start)
{ }