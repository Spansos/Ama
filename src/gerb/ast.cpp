#include "gerb/ast.hpp"

std::ostream & NonTerminalPostfix::print( std::ostream & stream ) {
    stream << "{ POSTFIX ";
    if ( std::holds_alternative<NonTerminalExpr*>( child ) )
        std::get<NonTerminalExpr*>( child )->print( stream );
    else
        stream << '<' << std::get<Token>( child ).capture << '>';
    stream << (op.has_value() ? op.value().capture : " ") << '}';
    return stream;
}

std::ostream & TerminalPrefix::print( std::ostream & stream ) {
    stream << "{ PREFIX ";
    stream << (op.has_value() ? op.value().capture : " ");
    if ( std::holds_alternative<TerminalExpr*>( child ) )
        std::get<TerminalExpr*>( child )->print( stream );
    else
        stream << '<' << std::get<Token>( child ).capture << '>' << '}';
    return stream;
}