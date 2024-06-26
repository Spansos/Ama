#include "gerb.hpp"

#include <iostream>

std::vector<Token> tokenize( const std::string & gerb ) {
    std::vector<Token> tokens;
    
    auto i = gerb.begin();
    while ( i <= gerb.end() ) {
        std::cout << *i << '\t' << i-gerb.begin() << std::endl;
        switch ( *i ) {
        case '(':
            tokens.push_back( Token{ .type=TokenType::BRACKET_OPEN, .capture="(" } );
            i++;
            break;
        case ')':
            tokens.push_back( Token{ .type=TokenType::BRACKET_CLOSE, .capture=")" } );
            i++;
            break;
        case '=':
            tokens.push_back( Token{ .type=TokenType::EQUALS, .capture="=" } );
            i++;
            break;
        case '|':
            tokens.push_back( Token{ .type=TokenType::BAR, .capture="|" } );
            i++;
            break;
        case '*':
            tokens.push_back( Token{ .type=TokenType::STAR, .capture="*" } );
            i++;
            break;
        case '+':
            tokens.push_back( Token{ .type=TokenType::PLUS, .capture="+" } );
            i++;
            break;
        case '?':
            tokens.push_back( Token{ .type=TokenType::QUESTION_MARK, .capture="?" } );
            i++;
            break;
        case '!':
            tokens.push_back( Token{ .type=TokenType::EXCLAMATION_MARK, .capture="!" } );
            i++;
            break;
        case '.':
            tokens.push_back( Token{ .type=TokenType::DOT, .capture="." } );
            i++;
            break;
        case '\0':
            tokens.push_back( Token{ .type=TokenType::END_OF_FILE, .capture="\0" } );
            i++;
            break;
        case '\n':
            tokens.push_back( Token{ .type=TokenType::NEWLINE, .capture="\n" } );
            i++;
            break;
        case ' ': case '\t':
            while ( *(++i) == ' ' || *i == '\t' );
            break;
        case ';':
            while ( *(i++) != '\n' );
            break;
        case '"': {
            auto begin = i;
            while ( *++i != '"' );
            i++;
            tokens.push_back( Token{ .type=TokenType::EXACT, .capture=std::string( begin, i ) } );
            break;
        }
        case '[': {
            auto begin = i;
            while ( *++i != ']' );
            i++;
            tokens.push_back( Token{ .type=TokenType::UNION, .capture=std::string( begin, i ) } );
            break;
        }
        case '\'': {
            auto begin = i;
            while ( *++i != '\'' );
            i++;
            tokens.push_back( Token{ .type=TokenType::TERMINAL, .capture=std::string( begin, i ) } );
            break;
        }
        default:
            auto begin = i;
            if ( *i >= 'A' && *i <= 'Z' ) {
                while ( *++i >= 'A' && *i <= 'Z' );
                i++;
                tokens.push_back( Token{ .type=TokenType::TERMINAL, .capture=std::string( begin, i ) } );
                break;
            }
            else if ( *i >= 'a' && *i <= 'z' ) {
                while ( *++i >= 'a' && *i <= 'z' );
                i++;
                tokens.push_back( Token{ .type=TokenType::NON_TERMINAL, .capture=std::string( begin, i ) } );
                break;
            }
            return tokens;
        }
    }

    return tokens;
}

Gerb gerb( const std::string & gerb ) {
    auto tokens = tokenize( gerb );

    std::cout << "TOKENS:" << '\n';
    for ( auto i : tokens ) {
        std::cout << i.capture << '\t' << static_cast<int>(i.type) << std::endl;
    }

    return Gerb{};
}