#include "token.hpp"

Token::Token (
    TokenType type,
    const std::string & code,
    int start,
    int length
) :
    start(start),
    length(length),
    type(type),
    _code(&code)
{ }

std::string Token::value( ) const {
    return _code->substr( start, length );
}

int Token::end( ) const {
    return start + length;
}