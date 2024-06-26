#include "gerb/parser.hpp"

bool match( std::vector<Token>::const_iterator & it, std::initializer_list<TokenType> types ) {
    for ( TokenType type : types )
        if ( it->type == type )
            return true;
    return false;
}

Gerb parse( const std::vector<Token> & tokens ) {

}