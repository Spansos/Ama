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

// bug where && and such gets interpretted as & & and such
std::vector<Token> lex (
    const std::string & code
) {
    std::array patterns = {
        TokenPattern{.allowed="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_", .match_type=TokenPattern::allowed_chars, .type=TokenType::identifier},
        TokenPattern{.allowed="*", .match_type=TokenPattern::exact_match, .type=TokenType::asterisk},
        TokenPattern{.allowed="&", .match_type=TokenPattern::exact_match, .type=TokenType::ampersand},
        TokenPattern{.allowed=":", .match_type=TokenPattern::exact_match, .type=TokenType::colon},
        TokenPattern{.allowed=".", .match_type=TokenPattern::exact_match, .type=TokenType::ampersand},
        TokenPattern{.allowed=",", .match_type=TokenPattern::exact_match, .type=TokenType::colon},
        TokenPattern{.allowed="0123456789", .match_type=TokenPattern::allowed_chars, .type=TokenType::int_},
        TokenPattern{.allowed="", .match_type=TokenPattern::other, .type=TokenType::float_},
        TokenPattern{.allowed="", .match_type=TokenPattern::other, .type=TokenType::string_},
        TokenPattern{.allowed="-", .match_type=TokenPattern::exact_match, .type=TokenType::minus},
        TokenPattern{.allowed="+", .match_type=TokenPattern::exact_match, .type=TokenType::plus},
        TokenPattern{.allowed="/", .match_type=TokenPattern::exact_match, .type=TokenType::divide},
        TokenPattern{.allowed="//", .match_type=TokenPattern::exact_match, .type=TokenType::idivide},
        TokenPattern{.allowed="%", .match_type=TokenPattern::exact_match, .type=TokenType::modulo},
        TokenPattern{.allowed="!", .match_type=TokenPattern::exact_match, .type=TokenType::not_},
        TokenPattern{.allowed="?", .match_type=TokenPattern::exact_match, .type=TokenType::qmark},
        TokenPattern{.allowed=">>", .match_type=TokenPattern::exact_match, .type=TokenType::rshift},
        TokenPattern{.allowed="<<", .match_type=TokenPattern::exact_match, .type=TokenType::lshift},
        TokenPattern{.allowed="|", .match_type=TokenPattern::exact_match, .type=TokenType::bor},
        TokenPattern{.allowed="^", .match_type=TokenPattern::exact_match, .type=TokenType::bxor},
        TokenPattern{.allowed="~", .match_type=TokenPattern::exact_match, .type=TokenType::bnot},
        TokenPattern{.allowed="-=", .match_type=TokenPattern::exact_match, .type=TokenType::minusa},
        TokenPattern{.allowed="+=", .match_type=TokenPattern::exact_match, .type=TokenType::plusa},
        TokenPattern{.allowed="/=", .match_type=TokenPattern::exact_match, .type=TokenType::dividea},
        TokenPattern{.allowed="//=", .match_type=TokenPattern::exact_match, .type=TokenType::idividea},
        TokenPattern{.allowed="%=", .match_type=TokenPattern::exact_match, .type=TokenType::moduloa},
        TokenPattern{.allowed="!=", .match_type=TokenPattern::exact_match, .type=TokenType::nota},
        TokenPattern{.allowed="?=", .match_type=TokenPattern::exact_match, .type=TokenType::qmarka},
        TokenPattern{.allowed=">>=", .match_type=TokenPattern::exact_match, .type=TokenType::rshifta},
        TokenPattern{.allowed="<<=", .match_type=TokenPattern::exact_match, .type=TokenType::lshifta},
        TokenPattern{.allowed="|=", .match_type=TokenPattern::exact_match, .type=TokenType::bora},
        TokenPattern{.allowed="^=", .match_type=TokenPattern::exact_match, .type=TokenType::bxora},
        TokenPattern{.allowed="~=", .match_type=TokenPattern::exact_match, .type=TokenType::bnota},
        TokenPattern{.allowed="(", .match_type=TokenPattern::exact_match, .type=TokenType::bropen},
        TokenPattern{.allowed=")", .match_type=TokenPattern::exact_match, .type=TokenType::brclose},
        TokenPattern{.allowed="[", .match_type=TokenPattern::exact_match, .type=TokenType::sqrbropen},
        TokenPattern{.allowed="]", .match_type=TokenPattern::exact_match, .type=TokenType::sqrbrclose},
        TokenPattern{.allowed="{", .match_type=TokenPattern::exact_match, .type=TokenType::curbropen},
        TokenPattern{.allowed="}", .match_type=TokenPattern::exact_match, .type=TokenType::curbrclose},
        TokenPattern{.allowed=" \n\t", .match_type=TokenPattern::allowed_chars, .type=TokenType::whitespace},
        TokenPattern{.allowed="", .match_type=TokenPattern::other, .type=TokenType::comment},
        TokenPattern{.allowed="\0", .match_type=TokenPattern::exact_match, .type=TokenType::eof}
    };

    std::vector<Token> tokens;
    
    auto i = code.begin( );
    while ( i != code.end( ) )
    {
        std::optional<Token> token;
        for ( auto pattern : patterns ) {
            std::optional<Token> match;
            if ( pattern.match_type == TokenPattern::other )
                switch ( pattern.type ) {
                case TokenType::float_:
                    break;
                case TokenType::string_:
                    break;
                case TokenType::comment:
                    break;
                default:
                    throw;
                }
            else
                match = pattern.match( code, i-code.begin() );
            if ( match.has_value() && ( !token.has_value() || match.value().length > token.value().length ) )
                token = match;
        }
        if ( !token.has_value() )
            throw;
        tokens.push_back( token.value() );
        i += token.value().length;
    }

    return tokens;
}