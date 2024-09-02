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
    IDENTIFIER,
    COLON,
    DOT,
    COMMA,
    SEMICOLON,
    INT,
    FLOAT,
    STRING,
    ASSIGNMENT,
    EQUAL,
    GREATER,
    LESSER,
    GREATER_EQUAL,
    LESSER_EQUAL,
    OR,
    AND,
    XOR,
    MINUS,
    PLUS,
    DIVIDE,
    INTIGER_DIVIDE,
    MODULUS,
    NOT,
    QUESTION_MARK,
    RIGHT_SHIGHT,
    LEFT_SHIFT,
    BIT_OR,
    BIT_AND,
    BIT_XOR,
    BIT_NOT,
    REFERENCE,
    DEREFERENCE,
    REFERNECE_ASSIGNMENT,
    DEREFERENCE_ASSIGNMENT,
    OR_ASSIGNMENT,
    AND_ASSIGNMENT,
    XOR_ASSIGNMENT,
    MINUS_ASSIGNMENT,
    PLUS_ASSIGNMENT,
    TIMES_ASSIGNMENT,
    DIVIDE_ASSIGNMENT,
    INTIGER_DIVIDE_ASSIGNMENT,
    MODULUS_ASSIGNMENT,
    NOT_ASSIGNMENT,
    QUESTION_MARK_ASSIGNMENT,
    RIGHT_SHIFT_ASSIGNMENT,
    LEFT_SHIFT_ASSIGNMENT,
    BINARY_OR_ASSIGNMENT,
    BINARY_AND_ASSIGNMENT,
    BINARY_XOR_ASSIGNMENT,
    BINARY_NOT_ASSIGNMENT,
    // brackets
    BRACKET_OPEN,
    BRACKET_CLOSE,
    SQUARE_BRACKET_OPEN,
    SQUARE_BRACKET_CLOSE,
    CURLY_BRACKET_OPEN,
    CURLY_BRACKET_CLOSE,
    // non-code
    WHITESPACE,
    COMMENT,
    END_OF_FILE
};


struct Token
{
    Token (
        TokenType type,
        int start
    );
    int start;
    TokenType type;
};