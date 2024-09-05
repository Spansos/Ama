#pragma once

#include "parser/expression.hpp"
#include "parser/statement.hpp"


std::expected<BodyNode*,ParseError> parse_body (
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

// ( (variable-declaration | expression)? ';' ) | for | while | do-while | func-definition | struct-definition | enum-definition | code-block

std::expected<StatementNode*,ParseError> parse_statement (
    std::vector<Token>::const_iterator & token_iterator
) {
    Token token = *token_iterator;
    switch (token.type) {
        case TokenType::REFERENCE:
        case TokenType::DEREFERENCE:
    }
}