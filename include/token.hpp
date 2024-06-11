#pragma once

#include <string>

// identifiers - [a-Z_][a-Z_0-9]*
// keywords - struct enum alias break continue while for if else switch case
// types - i8 i16 i32 i64 ui8 ui16 ui32 ui64 f32 f64
// type qualifiers - * & var
// operators - << >> - + * / // & | ^ ~ % ! ? :
// assignment - <<= >>= -= += *= /= //= &= |= ^= %= != ?= :
// brackets - ( ) { } [ ]
// comments - /*  */

enum class TokenType
{
    // types & variables
    identifier,
    asterisk,
    ampersand,
    colon,
    dot,
    comma,
    // literals
    int_,
    float_,
    string_,
    // operators
    minus,
    plus,
    divide,
    idivide,
    modulo,
    not_,
    qmark,
    rshift,
    lshift,
    bor,
    bxor,
    bnot,
    // assignment operators
    minusa,
    plusa,
    dividea,
    idividea,
    moduloa,
    nota,
    qmarka,
    rshifta,
    lshifta,
    bora,
    bxora,
    bnota,
    // brackets
    bropen,
    brclose,
    sqrbropen,
    sqrbrclose,
    curbropen,
    curbrclose,
    // non-code
    whitespace,
    comment,
    eof
};


class Token
{
public:
    Token (
        TokenType type,
        const std::string & code,
        int start,
        int length
    );
    std::string value( ) const;
    int end( ) const;
    int start, length;
    TokenType type;
private:
    const std::string * _code;
};