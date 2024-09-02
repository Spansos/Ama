#include "lexer.hpp"

#include <iostream>
#include <string>
#include <array>
#include <optional>

int token_length( std::string::const_iterator start, std::string allowed ) {
    int count = 0;
    while ( allowed.find(*start++) != std::string::npos ) count++;
    return count;
}

struct TokenPattern {
    std::string allowed;
    enum { allowed_chars, exact_match, other } match_type;
    TokenType type;
    std::optional<Token> match( const std::string & code, int start ) {
        switch ( match_type ) {
        case allowed_chars:
            return Token( type, code, start, token_length( code.begin()+start, allowed ) );
        case exact_match:
            if ( code.substr(start, allowed.length()) == allowed )
                return Token( type, code, start, allowed.length() );
            break;
        case other:
            throw;
        }
        return {};
    }
};

std::vector<Token> lex (
    const std::string & code
) {
    std::vector<Token> tokens;
    
    auto i = code.begin( );
    while ( i != code.end( ) )
    {
        
        tokens.push_back( token );
        i += token.value().length;
    }

    return tokens;
}