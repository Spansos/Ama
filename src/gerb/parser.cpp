#include "gerb/parser.hpp"

#include <optional>

#define RETURN_IF_ERROR( EXPR, IT, BEGIN ) do { auto value = EXPR; if (std::holds_alternative<ParseError>(value)) { IT = BEGIN; return std::get<ParseError>(value); } } while (0)

ParseError & operator|( ParseError & lhs, ParseError & rhs ) {
    if ( lhs.location > rhs.location )
        return lhs;
    return rhs;
}

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
    for ( auto i = types.begin(); i < types.end()-1; i++ )
        err += std::to_string( *i ) + " or ";
    err += std::to_string( *(types.end()-1) );
    return ParseError{ .error=err, .location=it };
}

std::variant<NonTerminalExpr, ParseError> non_terminal_expression( std::vector<Token>::const_iterator & it );

std::variant<NonTerminalPrimary, ParseError> non_terminal_primary( std::vector<Token>::const_iterator & it ) {
    auto begin = it;

    auto token = consume( it, { TokenType::TERMINAL, TokenType::NON_TERMINAL, TokenType::BRACKET_OPEN } );
    RETURN_IF_ERROR( token, it, begin );
    
    if ( std::get<Token>(token).type == TokenType::BRACKET_OPEN ) {
        auto expr = non_terminal_expression( it );
        RETURN_IF_ERROR( expr, it, begin );
        RETURN_IF_ERROR( consume( it, { TokenType::BRACKET_CLOSE } ), it, begin );
        return new NonTerminalExpr{std::get<NonTerminalExpr>(expr)};
    }
    return std::get<Token>(token);
}

std::variant<NonTerminalPostfix, ParseError> non_terminal_postfix( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    NonTerminalPostfix postfix;
    
    auto primary = non_terminal_primary( it );
    RETURN_IF_ERROR( primary, it, begin );
    postfix.child = std::get<NonTerminalPrimary>(primary);

    postfix.op = match( it, { TokenType::STAR, TokenType::PLUS, TokenType::QUESTION_MARK } );

    return postfix;
}

std::variant<NonTerminalOr, ParseError> non_terminal_or( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    NonTerminalOr term_or;
    auto postfix = non_terminal_postfix( it );
    RETURN_IF_ERROR( postfix, it, begin );
    term_or.children.push_back( std::get<NonTerminalPostfix>(postfix));
    
    while ( match( it, { TokenType::BAR } ) ) {
        auto child = non_terminal_postfix( it );
        RETURN_IF_ERROR( child, it, begin );
        term_or.children.push_back( std::get<NonTerminalPostfix>(child) );
    }

    return term_or;
}

std::variant<NonTerminalExpr, ParseError> non_terminal_expression( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    NonTerminalExpr expr;
    auto term_or = non_terminal_or( it );
    while ( std::holds_alternative<NonTerminalOr>(term_or) ) {
        expr.children.push_back( std::get<NonTerminalOr>(term_or) );
        term_or = non_terminal_or( it );
    }
    if ( expr.children.empty() ) {
        it = begin;
        return std::get<ParseError>(term_or);
    }
    return expr;
}

std::variant<NonTerminal, ParseError> non_terminal( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    NonTerminal nonterm;

    auto nonterm_token = consume( it, { TokenType::NON_TERMINAL } );
    RETURN_IF_ERROR( nonterm_token, it, begin );

    auto equal_token = consume( it, { TokenType::EQUALS } );
    RETURN_IF_ERROR( equal_token, it, begin );

    auto expr = non_terminal_expression( it );
    RETURN_IF_ERROR( expr, it, begin );
    
    nonterm.token = std::get<Token>( nonterm_token );
    nonterm.expression = std::get<NonTerminalExpr>( expr );

    return nonterm;
}

std::variant<TerminalExpr, ParseError> terminal_expression( std::vector<Token>::const_iterator & it );

std::variant<TerminalPrimary, ParseError> terminal_primary( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    auto token = consume( it, { TokenType::BRACKET_OPEN, TokenType::EXACT, TokenType::UNION, TokenType::DOT } );
    RETURN_IF_ERROR( token, it, begin );
    
    if ( std::get<Token>(token).type == TokenType::BRACKET_OPEN ) {
        auto expr = terminal_expression( it );
        RETURN_IF_ERROR( expr, it, begin );
        RETURN_IF_ERROR( consume( it, { TokenType::BRACKET_CLOSE } ), it, begin );
        return new TerminalExpr{std::get<TerminalExpr>(expr)};
    }
    return std::get<Token>(token);
}

std::variant<TerminalPrefix, ParseError> terminal_prefix( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    TerminalPrefix prefix;
    prefix.op = match( it, { TokenType::EXCLAMATION_MARK } );

    auto primary = terminal_primary( it );
    RETURN_IF_ERROR( primary, it, begin );
    prefix.child = std::get<TerminalPrimary>(primary);

    return prefix;
}

std::variant<TerminalPostfix, ParseError> terminal_postfix( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    TerminalPostfix postfix;
    
    auto prefix = terminal_prefix( it );
    RETURN_IF_ERROR( prefix, it, begin );
    postfix.child = std::get<TerminalPrefix>(prefix);

    postfix.op = match( it, { TokenType::STAR, TokenType::PLUS, TokenType::QUESTION_MARK } );

    return postfix;
}

std::variant<TerminalOr, ParseError> terminal_or( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    TerminalOr term_or;
    auto postfix = terminal_postfix( it );
    RETURN_IF_ERROR( postfix, it, begin );
    term_or.children.push_back( std::get<TerminalPostfix>(postfix));
    
    while ( match( it, { TokenType::BAR } ) ) {
        auto child = terminal_postfix( it );
        RETURN_IF_ERROR( child, it, begin );
        term_or.children.push_back( std::get<TerminalPostfix>(child) );
    }

    return term_or;
}

std::variant<TerminalExpr, ParseError> terminal_expression( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    TerminalExpr expr;
    auto term_or = terminal_or( it );
    while ( std::holds_alternative<TerminalOr>(term_or) ) {
        expr.children.push_back( std::get<TerminalOr>(term_or) );
        term_or = terminal_or( it );
    }
    if ( expr.children.empty() ) {
        it = begin;
        return std::get<ParseError>(term_or);
    }
    return expr;
}

std::variant<Terminal, ParseError> terminal( std::vector<Token>::const_iterator & it ) {
    auto begin = it;
    Terminal term;

    auto term_token = consume( it, { TokenType::TERMINAL } );
    RETURN_IF_ERROR( term_token, it, begin );

    auto equal_token = consume( it, { TokenType::EQUALS } );
    RETURN_IF_ERROR( equal_token, it, begin );

    auto expr = terminal_expression( it );
    RETURN_IF_ERROR( expr, it, begin );
    
    term.terminal = std::get<Token>( term_token );
    term.expression = std::get<TerminalExpr>( expr );

    return term;
}

std::variant<Gerb, ParseError> gerb( std::vector<Token>::const_iterator & it ) {
    Gerb gerb;

    while ( !match( it, { TokenType::END_OF_FILE } ) ) {
        std::optional<ParseError> error;
        auto term = terminal( it );
        if ( std::holds_alternative<Terminal>(term) ) {
            gerb.terminals.push_back( std::get<Terminal>(term) );
        } else {
            error = std::get<ParseError>(term);
            auto nonterm = non_terminal( it );
            if ( std::holds_alternative<NonTerminal>(nonterm) ) {
                gerb.non_terminals.push_back( std::get<NonTerminal>(nonterm) );
            } else {
                if ( error.value().location < std::get<ParseError>(nonterm).location )
                    error = std::get<ParseError>(nonterm);
            }
        }
        auto newline = consume(it, {TokenType::NEWLINE});
        if ( std::holds_alternative<ParseError>( newline ) ) {
            if ( error.has_value() )
                return error.value();
            return std::get<ParseError>( newline );
        }
    }

    return gerb;
}


std::variant<Gerb, ParseError> parse( const std::vector<Token> & tokens ) {
    auto it = tokens.begin();
    return gerb( it );
}