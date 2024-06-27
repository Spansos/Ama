#include "gerb/parser.hpp"

#include <optional>

struct ParseError {
    std::string error;
    std::vector<Token>::const_iterator location;
};

std::optional<Token> match( std::vector<Token>::const_iterator & it, std::initializer_list<TokenType> types ) {
    for ( TokenType type : types )
        if ( it->type == type )
            return *it++;
    return {};
}

std::variant<Token, ParseError> consume( std::vector<Token>::const_iterator & it, std::initializer_list<TokenType> types ) {
    auto token = match( it, types );
    if ( token )
        return token.value();

    std::string err = "expected ";
    for ( auto type : types )
        err += std::to_string( static_cast<int>(type) ) + ", ";
    err += "at " + it->capture;
    return ParseError{ .error=err, .location=it };
}

std::optional<TerminalOr> terminal_or( std::vector<Token>::const_iterator & it ) {
    TerminalOr term_or;
    auto postfix = terminal_postfix( it );
    while ( match( it, { TokenType::BAR } ) ) {
        auto child = terminal_postfix( it );
        term_or.children.push_back(  );
    }
}


std::optional<TerminalExpr> terminal_expression( std::vector<Token>::const_iterator & it ) {
    TerminalExpr expr;
    auto term_or = terminal_or( it );
    while ( term_or ) {
        expr.children.push_back( term_or );
        term_or = terminal_or( it );
    }
    if ( expr.children.empty() )
        return {};
    return expr;
}

std::optional<Terminal> terminal( std::vector<Token>::const_iterator & it ) {
    Terminal term;

    term.terminal = consume( it, { TokenType::TERMINAL } );
    consume( it, { TokenType::EQUALS } );
    auto expr = terminal_expression( it );
    if ( expr ) {
        term.expression = expr.value();
        return term;
    }
    return { };
}

std::optional<Gerb> gerb( std::vector<Token>::const_iterator & it ) {
    Gerb gerb;

    while ( !match( it, { TokenType::END_OF_FILE } ) ) {
        auto term = terminal( it );
        if ( term )
            gerb.terminals.push_back( term.value() );
        else if ( auto nonterm = non_terminal( it ) )
            gerb.non_terminals.push_back( nonterm.value() );
        consume( it, { TokenType::NEWLINE } );
    }
    consume( it, { TokenType::END_OF_FILE } );

    return gerb;
}

Gerb parse( const std::vector<Token> & tokens ) {
    auto it = tokens.begin();
    return gerb( it ).value();
}