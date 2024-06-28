#include "gerb/parser.hpp"

#include <optional>

struct ParseError {
    std::string error;
    std::vector<Token>::const_iterator location;
};

#define RETURN_IF_ERROR( EXPR ) do { auto value = EXPR; if (std::holds_alternative<ParseError>(value)) return std::get<ParseError>(value); } while (0)

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
    for ( auto type : types )
        err += std::to_string( static_cast<int>(type) ) + ", ";
    err += "at " + it->capture;
    return ParseError{ .error=err, .location=it };
}

std::variant<NonTerminal, ParseError> non_terminal( std::vector<Token>::const_iterator & it ) {
    NonTerminal non_term;
    while ( (++it)->type != TokenType::END_OF_FILE );
    return non_term;
}

std::variant<TerminalExpr, ParseError> terminal_expression( std::vector<Token>::const_iterator & it );

std::variant<TerminalPrimary, ParseError> terminal_primary( std::vector<Token>::const_iterator & it ) {
    auto token = consume( it, { TokenType::BRACKET_OPEN, TokenType::EXACT, TokenType::UNION, TokenType::DOT } );
    RETURN_IF_ERROR( token );
    
    if ( std::get<Token>(token).type == TokenType::BRACKET_OPEN ) {
        auto expr = terminal_expression( it );
        RETURN_IF_ERROR( expr );
        RETURN_IF_ERROR( consume( it, { TokenType::BRACKET_CLOSE } ) );
        return std::make_unique<TerminalExpr>( std::get<TerminalExpr>(expr) );
    }
    return std::get<Token>(token);
}

std::variant<TerminalPrefix, ParseError> terminal_prefix( std::vector<Token>::const_iterator & it ) {
    TerminalPrefix prefix;
    prefix.op = match( it, { TokenType::EXCLAMATION_MARK } );

    auto primary = terminal_primary( it );
    if ( std::holds_alternative<ParseError>(primary) )
        return std::get<ParseError>(primary);
    prefix.child = std::move(std::get<TerminalPrimary>(primary));

    return prefix;
}

std::variant<TerminalPostfix, ParseError> terminal_postfix( std::vector<Token>::const_iterator & it ) {
    TerminalPostfix postfix;
    
    auto prefix = terminal_prefix( it );
    if ( std::holds_alternative<ParseError>(prefix) )
        return std::get<ParseError>(prefix);
    postfix.child = std::move(std::get<TerminalPrefix>(prefix));

    postfix.op = match( it, { TokenType::STAR, TokenType::PLUS, TokenType::QUESTION_MARK } );

    return postfix;
}

std::variant<TerminalOr, ParseError> terminal_or( std::vector<Token>::const_iterator & it ) {
    TerminalOr term_or;
    auto postfix = terminal_postfix( it );
    if ( std::holds_alternative<ParseError>(postfix) )
        return std::get<ParseError>(postfix);
    term_or.children.push_back( std::move(std::get<TerminalPostfix>(postfix)) );
    
    while ( match( it, { TokenType::BAR } ) ) {
        auto child = terminal_postfix( it );
        if ( std::holds_alternative<ParseError>(child) )
            return std::get<ParseError>(child);
        term_or.children.push_back( std::move(std::get<TerminalPostfix>(child)) );
    }

    return term_or;
}


std::variant<TerminalExpr, ParseError> terminal_expression( std::vector<Token>::const_iterator & it ) {
    TerminalExpr expr;
    auto term_or = terminal_or( it );
    while ( std::holds_alternative<TerminalOr>(term_or) ) {
        expr.children.push_back( std::move(std::get<TerminalOr>(term_or)) );
        term_or = terminal_or( it );
    }
    if ( expr.children.empty() )
        return std::get<ParseError>(term_or);
    return expr;
}

std::variant<Terminal, ParseError> terminal( std::vector<Token>::const_iterator & it ) {
    Terminal term;

    auto term_token = consume( it, { TokenType::TERMINAL } );
    if ( std::holds_alternative<ParseError>(term_token) )
        return std::get<ParseError>( term_token );

    auto equal_token = consume( it, { TokenType::EQUALS } );
    if ( std::holds_alternative<ParseError>(equal_token) )
        return std::get<ParseError>( equal_token );

    auto expr = terminal_expression( it );
    if ( std::holds_alternative<ParseError>(expr) )
        return std::get<ParseError>( expr );
    
    term.terminal = std::get<Token>( term_token );
    term.expression = std::get<TerminalExpr>( expr );

    return { };
}

std::variant<Gerb, ParseError> gerb( std::vector<Token>::const_iterator & it ) {
    Gerb gerb;

    while ( !match( it, { TokenType::END_OF_FILE } ) ) {
        auto term = terminal( it );
        if ( !std::holds_alternative<ParseError>(term) )
            gerb.terminals.push_back( std::get<Terminal>(term) );
        else {
            auto nonterm = non_terminal( it );
            if ( !std::holds_alternative<ParseError>(nonterm) )
                gerb.non_terminals.push_back( std::get<NonTerminal>(nonterm) );
            else
                return std::get<ParseError>(term) | std::get<ParseError>(nonterm);
        }
        RETURN_IF_ERROR( consume(it, {TokenType::NEWLINE}) );
    }
    RETURN_IF_ERROR( consume(it, {TokenType::END_OF_FILE}) );

    return gerb;
}

Gerb parse( const std::vector<Token> & tokens ) {
    auto it = tokens.begin();
    return std::get<Gerb>(gerb( it ));
}