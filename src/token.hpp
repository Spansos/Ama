#pragma once

#include <regex>
#include <string>

class TokenType
{
public:
    TokenType (
        std::string name,
        std::regex regex
    );
    bool operator==(
        const TokenType & lhs
    ) const;
    std::string name( ) const;
    std::regex regex( ) const;
private:
    std::string _name;
    std::regex _regex;
};


class Token
{
public:
    Token (
        const TokenType & type,
        const std::string & code,
        int start=0
    );
    Token( );
    int start( ) const;
    int length( ) const;
    int end( ) const;
    std::string value( ) const;
    TokenType type( ) const;
private:
    std::string _value;
    const TokenType * _type;
    int _start;
};