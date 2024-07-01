#pragma once

#include <iostream>
#include <variant>
#include <vector>
#include <optional>
#include <memory>
#include "gerb/token.hpp"

struct NonTerminalExpr;
typedef std::variant<NonTerminalExpr*, Token> NonTerminalPrimary;

struct NonTerminalPostfix {
    NonTerminalPrimary child;
    std::optional<Token> op;
    std::ostream & print( std::ostream & stream );
};

struct NonTerminalOr {
    std::vector<NonTerminalPostfix> children;
    std::ostream & print( std::ostream & stream ) {
        stream << "{ OR ";
        for ( auto & i : children )
            i.print( stream );
        return stream << '}';
    }
};

struct NonTerminalExpr {
    std::vector<NonTerminalOr> children;
    std::ostream & print( std::ostream & stream ) {
        stream << "{ EXPR ";
        for ( auto & i : children )
            i.print( stream );
        return stream << '}';
    }
};

struct NonTerminal {
    Token token;
    NonTerminalExpr expression;
    std::ostream & print( std::ostream & stream ) {
        stream << "{ NONTERM <" << token.capture << '>';
        return expression.print( stream );
    }
};

struct TerminalExpr;
typedef std::variant<TerminalExpr*, Token> TerminalPrimary;

struct TerminalPrefix {
    TerminalPrimary child;
    std::optional<Token> op;
    std::ostream & print( std::ostream & stream );
};

struct TerminalPostfix {
    TerminalPrefix child;
    std::optional<Token> op;
    std::ostream & print( std::ostream & stream ) {
        stream << "{ POSTFIX ";
        child.print( stream );
        stream << (op.has_value() ? op.value().capture : " ") << '}';
        return stream;
    }
};

struct TerminalOr {
    std::vector<TerminalPostfix> children;
    std::ostream & print( std::ostream & stream ) {
        stream << "{ OR ";
        for ( auto & i : children )
            i.print( stream );
        return stream << '}';
    }
};

struct TerminalExpr {
    std::vector<TerminalOr> children;
    std::ostream & print( std::ostream & stream ) {
        stream << "{ EXPR ";
        for ( auto & i : children )
            i.print( stream );
        return stream << '}';
    }
};

struct Terminal {
    Token terminal;
    TerminalExpr expression;
    std::ostream & print( std::ostream & stream ) {
        stream << "{ TERM <" << terminal.capture << '>';
        return expression.print( stream );
    }
};

struct Gerb {
    std::vector<Terminal> terminals;
    std::vector<NonTerminal> non_terminals;
    std::ostream & print( std::ostream & stream ) {
        stream << "TERMINALS:\n";
        for ( auto & term : terminals )
            term.print( stream << "  " ) << '\n';
        stream << "NON TERMINALS:\n";
        for ( auto & nonterm : non_terminals )
            nonterm.print( stream << "  " ) << '\n';
        return stream;
    }
};