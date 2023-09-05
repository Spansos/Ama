#include "lexer.hpp"


Lexer::Lexer(
    std::vector<TokenType> tokenTypes
) :
    _tokenTypes( tokenTypes )
{ }

std::vector<Token> Lexer::lex(
    const std::string & code
) const
{
    std::vector< Token > tokens;
    
    std::string::const_iterator i = code.begin( );
    while ( i != code.end( ) )
    {
        Token token;
        for ( const TokenType & type : _tokenTypes )
        {
            Token temptoken( type, code, i - code.begin( ) );
            token = token.length( ) > temptoken.length( ) ? token : temptoken;
        }
        tokens.push_back( token );
        i += token.length( );
    }

    return tokens;
}