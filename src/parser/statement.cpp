#include "parser/expression.hpp"
#include "parser/statement.hpp"


// code-block  = { statement* }
std::expected<BlockNode*,ParseError> parse_block (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    (void)code;

    BlockNode * node = new BlockNode;
    
    auto begin_it = token_iterator;
    
    auto bracket = match(begin_it, token_iterator, {TokenType::CURLY_BRACKET_OPEN});
    if (!bracket)
        return std::unexpected{bracket.error()};
    
    while (auto statement = parse_statement(token_iterator))
        node->statements.push_back(statement.value());
    
    bracket = match(begin_it, token_iterator, {TokenType::CURLY_BRACKET_CLOSE});
    if (!bracket)
        return std::unexpected{bracket.error()};

    return node;
}

// type        = 'var'? ( ( '&'|'*' type ) | IDENTIFIER )
std::expected<TypeNode*,ParseError> parse_type (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    auto begin_it = token_iterator;
    
    // var
    bool is_var = false;
    auto var = check(token_iterator, {TokenType::IDENTIFIER});
    if (var && token_content(var.value(), code) == "var") {
        is_var = true;
        token_iterator++;
    }

    // & or *
    if (auto token = check(token_iterator, {TokenType::REFERENCE, TokenType::DEREFERENCE})) {
        token_iterator++;
        auto sub_node = parse_type(token_iterator, code);
        if (sub_node) {
            if (token->type == TokenType::REFERENCE)
                return new TypeNode{
                    .var = is_var,
                    .reference = sub_node.value(),
                    .type = TypeNode::REFERENCE
                };
            else
                return new TypeNode{
                    .var = is_var,
                    .pointer = sub_node.value(),
                    .type = TypeNode::POINTER
                };
        }
    }

    // IDENTIFIER
    if (auto token = match(begin_it, token_iterator, {TokenType::IDENTIFIER}))
        return new TypeNode{
            .var = is_var,
            .identifier = token.value(),
            .type = TypeNode::IDENTIFIER
        };
    
    return std::unexpected{ParseError::INVALID_TYPE};
}

// enum-definition = 'enum' IDENTIFIER '{' (type? IDENTIFIER ';')* '}'
std::expected<EnumNode*,ParseError> parse_enum_definition (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    auto begin_it = token_iterator;
    
    auto enum_ = check(token_iterator, {TokenType::IDENTIFIER});
    if (enum_ && token_content(enum_.value(), code) == "enum")
        token_iterator++;
    else
        return std::unexpected{ParseError::INVALID_TYPE};

    auto identifier = match(begin_it, token_iterator, {TokenType::IDENTIFIER});
    if (!identifier)
        return std::unexpected{ParseError::INVALID_TYPE};

    if (!match(begin_it, token_iterator, {TokenType::CURLY_BRACKET_OPEN}))
        return std::unexpected{ParseError::INVALID_TYPE};

    std::vector<DeclarationNode*> declarations;
    while (auto declaration = parse_declaration(token_iterator, code)) {
        declarations.push_back(declaration.value());
    }

    if (!match(begin_it, token_iterator, {TokenType::CURLY_BRACKET_CLOSE}))
        return std::unexpected{ParseError::INVALID_TYPE};

    return std::unexpected{ParseError::INVALID_TYPE};
}

// struct-definition = 'struct' IDENTIFIER '{' (variable-declaration ';')* '}'
std::expected<StructNode*,ParseError> parse_struct_definition (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    
}

// func-definition = type IDENTIFIER '(' arguments ')' 'var'? statement
std::expected<FuncNode*,ParseError> parse_func_definition (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    
}

// variable-declaration = type IDENTIFIER ('=' expression)?
std::expected<VarDeclNode*,ParseError> parse_var_decl (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    
}

std::expected<DeclarationNode*,ParseError> parse_declaration (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    
}

// do-while    = 'do' statement 'while' '(' expression ')'
std::expected<DoWhileNode*,ParseError> parse_do_while (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {

}

// while       = 'while' '(' expression ')' statement
std::expected<WhileNode*,ParseError> parse_while (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    
}


// for         = 'for' '(' statement expression ';' statement ')' statement
std::expected<ForNode*,ParseError> parse_for (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    
}

// statement   = ( (variable-declaration | expression)? ';' ) | for | while | do-while | func-definition | struct-definition | enum-definition | code-block
std::expected<StatementNode*,ParseError> parse_statement (
    std::vector<Token>::const_iterator & token_iterator,
    const std::string & code
) {
    // should be changed when proper errors.
    Token token = *token_iterator;
    switch (token.type) {
        case TokenType::REFERENCE:
        case TokenType::DEREFERENCE: {
            StatementNode * r = new StatementNode{
                .var_decl = parse_var_decl(token_iterator, code).value(),
                .type = StatementNode::VAR_DECL
            };
            if (token_iterator++->type != TokenType::SEMICOLON)
                throw;
            return r;
        }
        case TokenType::CURLY_BRACKET_OPEN:
            return new StatementNode{
                .code_block = parse_block(token_iterator, code).value(),
                .type = StatementNode::CODE_BLOCK
            };
        default:
            
    }
}