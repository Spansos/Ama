#pragma once

#include <string>
#include <variant>
#include <vector>
#include <optional>

enum class TokenType {
    BRACKET_OPEN, BRACKET_CLOSE, EQUALS, BAR, STAR, PLUS, QUESTION_MARK, EXCLAMATION_MARK, DOT, END_OF_FILE, NEWLINE, EXACT, UNION, NON_TERMINAL, TERMINAL
};

struct Token {
    TokenType type;
    std::string capture;
};

struct NonTerminalExpr;
typedef std::variant<NonTerminalExpr*, Token> NonTerminalPrimary;

struct NonTerminalPostfix {
    NonTerminalPrimary child;
    std::optional<Token> op;
};

struct NonTerminalOr {
    std::vector<NonTerminalPostfix> children;
};

struct NonTerminalExpr {
    std::vector<NonTerminalOr> children;
};

struct NonTerminal {
    Token token;
    NonTerminalExpr expression;
};

struct TerminalExpr;
typedef std::variant<TerminalExpr*, Token> TerminalPrimary;

struct TerminalPrefix {
    TerminalPrimary child;
    std::optional<TokenType> op;
};

struct TerminalPostfix {
    TerminalPrefix child;
    std::optional<TokenType> op;
};

struct TerminalOr {
    std::vector<TerminalPostfix> children;
};

struct TerminalExpr {
    std::vector<TerminalOr> children;
};

struct Terminal {
    Token terminal;
    TerminalExpr expression;
};

struct Gerb {
    std::vector<Terminal> terminals;
    std::vector<NonTerminal> non_terminals;
};