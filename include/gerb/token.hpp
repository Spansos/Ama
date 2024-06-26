#pragma once

#include <string>

enum class TokenType {
    BRACKET_OPEN, BRACKET_CLOSE, EQUALS, BAR, STAR, PLUS, QUESTION_MARK, EXCLAMATION_MARK, DOT, END_OF_FILE, NEWLINE, EXACT, UNION, NON_TERMINAL, TERMINAL
};

struct Token {
    TokenType type;
    std::string capture;
};