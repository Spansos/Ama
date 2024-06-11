#include "lexer.hpp"
#include <iostream>

int main( )
{
    std::string code = "test*for<<-+   12414 wwwas..dfd sad829 h87efy";
    auto tokens = lex( code );

    for ( const auto & token : tokens )
        std::cout
        << '<'
        << static_cast<int>(token.type)
        << ':'
        << token.value()
        << '>'
        << '\n';

    return 0;
}