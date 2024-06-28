#pragma once

#include <variant>
#include <vector>
#include <optional>
#include <memory>
#include "gerb/token.hpp"

struct NonTerminalExpr;
typedef std::variant<std::unique_ptr<NonTerminalExpr>, Token> NonTerminalPrimary;

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
typedef std::variant<std::unique_ptr<TerminalExpr>, Token> TerminalPrimary;

struct TerminalPrefix {
    TerminalPrimary child;
    std::optional<Token> op;
};

struct TerminalPostfix {
    TerminalPrefix child;
    std::optional<Token> op;
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