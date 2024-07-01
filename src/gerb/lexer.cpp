#include "gerb/lexer.hpp"

std::vector<Token> tokenize( const std::string & gerb ) {
    std::vector<Token> tokens;
    
    auto i = gerb.begin();
    while ( i <= gerb.end() ) {
        switch ( *i ) {
        case '(':
            tokens.push_back( Token{ .type=TokenType::BRACKET_OPEN, .capture="(", .start=i } );
            i++;
            break;
        case ')':
            tokens.push_back( Token{ .type=TokenType::BRACKET_CLOSE, .capture=")", .start=i } );
            i++;
            break;
        case '=':
            tokens.push_back( Token{ .type=TokenType::EQUALS, .capture="=", .start=i } );
            i++;
            break;
        case '|':
            tokens.push_back( Token{ .type=TokenType::BAR, .capture="|", .start=i } );
            i++;
            break;
        case '*':
            tokens.push_back( Token{ .type=TokenType::STAR, .capture="*", .start=i } );
            i++;
            break;
        case '+':
            tokens.push_back( Token{ .type=TokenType::PLUS, .capture="+", .start=i } );
            i++;
            break;
        case '?':
            tokens.push_back( Token{ .type=TokenType::QUESTION_MARK, .capture="?", .start=i } );
            i++;
            break;
        case '!':
            tokens.push_back( Token{ .type=TokenType::EXCLAMATION_MARK, .capture="!", .start=i } );
            i++;
            break;
        case '.':
            tokens.push_back( Token{ .type=TokenType::DOT, .capture=".", .start=i } );
            i++;
            break;
        case '\0':
            tokens.push_back( Token{ .type=TokenType::END_OF_FILE, .capture="\0", .start=i } );
            i++;
            break;
        case '\n':
            tokens.push_back( Token{ .type=TokenType::NEWLINE, .capture="\n", .start=i } );
            i++;
            break;
        case ' ': case '\t':
            while ( *++i == ' ' || *i == '\t' );
            break;
        case ';':
            while ( *(i++) != '\n' );
            break;
        case '"': {
            auto begin = i;
            while ( *(i++) == '\\' || *i != '"' );
            i++;
            tokens.push_back( Token{ .type=TokenType::EXACT, .capture=std::string( begin, i ), .start=begin } );
            break;
        }
        case '[': {
            auto begin = i;
            while ( *++i != ']' );
            i++;
            tokens.push_back( Token{ .type=TokenType::UNION, .capture=std::string( begin, i ), .start=begin } );
            break;
        }
        case '\'': {
            auto begin = i;
            while ( *++i != '\'' );
            i++;
            tokens.push_back( Token{ .type=TokenType::TERMINAL, .capture=std::string( begin, i ), .start=begin } );
            break;
        }
        default:
            auto begin = i;
            if ( ( *i >= 'A' && *i <= 'Z' ) || *i == '_' ) {
                while ( ( *++i >= 'A' && *i <= 'Z' ) || *i == '_' );
                tokens.push_back( Token{ .type=TokenType::TERMINAL, .capture=std::string( begin, i ), .start=begin } );
                break;
            }
            else if ( ( *i >= 'a' && *i <= 'z' ) || *i == '-' ) {
                while ( ( *++i >= 'a' && *i <= 'z' ) || *i == '-' );
                tokens.push_back( Token{ .type=TokenType::NON_TERMINAL, .capture=std::string( begin, i ), .start=begin } );
                break;
            }
            return tokens;
        }
    }

    return tokens;
}