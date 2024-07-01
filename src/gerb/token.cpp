#include "gerb/token.hpp"

namespace std {

 std::string to_string( TokenType type ) {
    switch ( type ) {
        case TokenType::BRACKET_OPEN:
            return "BRACKET_OPEN";
        case TokenType::BRACKET_CLOSE:
            return "BRACKET_CLOSE";
        case TokenType::EQUALS:
            return "EQUALS";
        case TokenType::BAR:
            return "BAR";
        case TokenType::STAR:
            return "STAR";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::QUESTION_MARK:
            return "QUESTION_MARK";
        case TokenType::EXCLAMATION_MARK:
            return "EXCLAMATION_MARK";
        case TokenType::DOT:
            return "DOT";
        case TokenType::END_OF_FILE:
            return "END_OF_FILE";
        case TokenType::NEWLINE:
            return "NEWLINE";
        case TokenType::EXACT:
            return "EXACT";
        case TokenType::UNION:
            return "UNION";
        case TokenType::NON_TERMINAL:
            return "NON_TERMINAL";
        case TokenType::TERMINAL:
            return "TERMINAL";
    }
    return "TOKEN_ERROR";
}

}