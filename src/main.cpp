#include "lexer.hpp"
#include <iostream>

int main( )
{
    TokenType number( "number", std::regex( "[1-9]*" ) );
    TokenType alphabetical( "alpha", std::regex( "[a-zA-Z]*" ) );
    TokenType whitespace( "whitespace", std::regex( "[ \n\t]*" ) );

    Lexer lexer( { number, alphabetical, whitespace } );

    auto tokens = lexer.lex( "test   12414 wwwas..dfd sad829 h87efy" );

    for ( const auto & token : tokens )
        std::cout
        << '<'
        << token.type().name()
        << ':'
        << token.value()
        << '>'
        << '\n';

    return 0;
}