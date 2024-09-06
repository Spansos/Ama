#pragma once

#include "parser/expression.hpp"
#include "parser/statement.hpp"


std::expected<BlockNode*,ParseError> parse_block (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<TypeNode*,ParseError> parse_type (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<EnumNode*,ParseError> parse_enum_definition (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<StructNode*,ParseError> parse_struct_definition (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<FuncNode*,ParseError> parse_func_definition (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<VarDeclNode*,ParseError> parse_var_decl (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<Declaration*,ParseError> parse_declaration (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<DoWhileNode*,ParseError> parse_do_while (
    std::vector<Token>::const_iterator & token_iterator
) {

}

std::expected<WhileNode*,ParseError> parse_while (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

std::expected<ForNode*,ParseError> parse_for (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}

// enum-definition = 'enum' IDENTIFIER '{' (type? IDENTIFIER ';')* '}'
// struct-definition = 'struct' IDENTIFIER '{' (variable-declaration ';')* '}'
// func-definition = type IDENTIFIER '(' arguments ')' 'var'? statement
// do-while    = 'do' statement 'while' '(' expression ')'
// while       = 'while' '(' expression ')' statement
// for         = 'for' '(' statement expression ';' statement ')' statement
// type        = 'var'? ( ( '&'|'*' type ) | IDENTIFIER )
// variable-declaration = type IDENTIFIER ('=' expression)?
// code-block  = { statement* }

// statement   = ( (variable-declaration | expression)? ';' ) | for | while | do-while | func-definition | struct-definition | enum-definition | code-block
std::expected<StatementNode*,ParseError> parse_statement (
    std::vector<Token>::const_iterator & token_iterator
) {
    // should be changed when proper errors.
    Token token = *token_iterator;
    switch (token.type) {
        case TokenType::REFERENCE:
        case TokenType::DEREFERENCE: {
            StatementNode * r = new StatementNode{
                .var_decl = parse_var_decl(token_iterator).value(),
                .type = StatementNode::VAR_DECL
            };
            if (token_iterator++->type != TokenType::SEMICOLON)
                throw;
            return r;
        }
        case TokenType::CURLY_BRACKET_OPEN:
            return new StatementNode{
                .code_block = parse_block(token_iterator).value(),
                .type = StatementNode::CODE_BLOCK
            };
    }
}