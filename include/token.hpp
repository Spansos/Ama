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
    // general or various uses
    colon,
    dot,
    comma,
    semicolon,
    // literals
    int_,
    float_,
    string_,
    // operators
    asignment,
    equal,
    greater,
    lesser,
    greaterequal,
    lesserequal,
    or_,
    and_,
    xor_,
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
    band,
    bxor,
    bnot,
    // assignment operators
    ora,
    anda,
    xora,
    minusa,
    plusa,
    timesa,
    dividea,
    idividea,
    moduloa,
    nota,
    qmarka,
    rshifta,
    lshifta,
    bora,
    banda,
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