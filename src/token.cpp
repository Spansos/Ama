#include <token.hpp>

TokenType::TokenType(
    std::string name,
    std::regex regex
) :
    _name ( name ),
    _regex ( regex )
{ }

bool TokenType::operator==(
    const TokenType & lhs
) const
{
    return _name == lhs.name( );
}

std::string TokenType::name( ) const {
    return _name;
}

std::regex TokenType::regex( ) const {
    return _regex;
}



Token::Token(
    const TokenType & type,
    const std::string & code,
    int start
) :
    _type( &type )
{
    std::match_results<std::string::const_iterator> results;
    std::regex_search(
        code.begin( )+start,
        code.end(),
        results,
        _type->regex( ),
        std::regex_constants::match_continuous
    );
    
    _value = results[0];
    _start = start;
}

Token::Token( ) :
    _value( "" ),
    _type( NULL ),
    _start( 0 )
{ }

int Token::start( ) const
{
    return _start;
}

int Token::length( ) const
{
    return _value.length( );
}

int Token::end( ) const
{
    return _start + _value.length( );
}

std::string Token::value( ) const
{
    return _value;
}

TokenType Token::type( ) const
{
    return *_type;
}