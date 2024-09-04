#include "parser.hpp"

#include <expected>
#include <iostream>

enum class ParseError {

};

// binary-binary   = binary-shift ( ( '|' | '&' | '^' ) binary-shift )*
// binary-shift    = binary-add ( ( '<<' | '>>' ) binary-add )*
// binary-add      = binary-mult ( ( '+' | '-' ) binary-mult )*
// binary-mult     = unary-prefix ( ( '*' | '/' | '//' | '%' ) unary-prefix )*
// unary-prefix    = ( '*' | '&' | '-' | '+' | '!' | '~' ) unary-prefix | unary-postfix
// unary-postfix   = primary '(' arguments ')' | primary '[' expression ']' | primary
// primary         = INT | FLOAT | STRING | IDENTIFIER | '(' expression ')'

std::expected<ExpressionNode*,ParseError> parse_expression (
    std::vector<Token>::const_iterator & token_iterator
);

std::expected<Primary*, ParseError> parse_primary(
    std::vector<Token>::const_iterator & token_iterator
) {
    switch (token_iterator->type) {
        case TokenType::INT:
        case TokenType::FLOAT:
        case TokenType::STRING:
        case TokenType::IDENTIFIER:
            return new Primary{
                .literal = *token_iterator++,
                .type = Primary::LITERAL
            };
        case TokenType::BRACKET_OPEN: {
            token_iterator++;
            Primary * r = new Primary{
                .expression = parse_expression(token_iterator).value(),
                .type = Primary::EXPRESSION
            };
            token_iterator++;
            return r;
        }
        default:
            return std::unexpected{ParseError{}};
    }
}

std::expected<std::vector<ExpressionNode*>,ParseError> parse_arguments(
    std::vector<Token>::const_iterator & token_iterator
) {
    // guard clause. should be changed when parsing error actually be existing
    if (token_iterator->type == TokenType::BRACKET_CLOSE)
        return {};
    
    std::vector<ExpressionNode*> arguments;
    arguments.push_back(parse_expression(token_iterator).value());
    while (token_iterator++->type == TokenType::COMMA)
        arguments.push_back(parse_expression(token_iterator).value());

    return arguments;
}

std::expected<UnaryPostfix*, ParseError> parse_unary_postfix(
    std::vector<Token>::const_iterator & token_iterator
) {
    Primary * primary = parse_primary(token_iterator).value();
    switch ((++token_iterator)->type) {
        case TokenType::BRACKET_OPEN:
            token_iterator++;
            return new UnaryPostfix{
                .function = UnaryPostfix::Function{
                    .node = primary,
                    .arguments = parse_arguments(token_iterator).value()
                },
                .type = UnaryPostfix::FUNCTION
            };
        case TokenType::SQUARE_BRACKET_OPEN:
            token_iterator++;
            return new UnaryPostfix{
                .index = UnaryPostfix::Index{
                    .node = primary,
                    .index = parse_expression(token_iterator).value()
                },
                .type = UnaryPostfix::INDEX
            };
        default:
            return new UnaryPostfix{
                .nop = primary,
                .type = UnaryPostfix::NOP
            };
    }
}


std::expected<UnaryPrefix*, ParseError> parse_unary_prefix(
    std::vector<Token>::const_iterator & token_iterator
) {
    Token token = *token_iterator;
    if (token.type == TokenType::REFERENCE || token.type == TokenType::DEREFERENCE || token.type == TokenType::MINUS || token.type == TokenType::PLUS || token.type == TokenType::NOT || token.type == TokenType::BIT_NOT) {
        token_iterator++;
        return new UnaryPrefix{
            .prefix = UnaryPrefix::Prefix{
                .node = parse_unary_prefix(token_iterator).value(),
                .op = token
            },
            .type = UnaryPrefix::PREFIX
        };
    }

    return new UnaryPrefix{
        .nop = parse_unary_postfix(token_iterator).value(),
        .type = UnaryPrefix::NOP
    };
}


std::expected<BinaryMultNode*, ParseError> parse_binary_mult(
    std::vector<Token>::const_iterator & token_iterator
) {
    auto lhs = parse_unary_prefix(token_iterator);

    Token token = *token_iterator;
    if (token.type == TokenType::TIMES || token.type == TokenType::DIVIDE || token.type == TokenType::INTIGER_DIVIDE || token.type == TokenType::MODULUS) {
        token_iterator++;
        return new BinaryMultNode{
            .lhs = lhs.value(),
            .rhs = parse_binary_mult(token_iterator).value(),
            .op = token
        };
    }

    return new BinaryMultNode{
        .lhs = lhs.value(),
        .rhs = nullptr,
        .op = Token{}
    };
}


std::expected<BinaryAddNode*, ParseError> parse_binary_add(
    std::vector<Token>::const_iterator & token_iterator
) {
    auto lhs = parse_binary_mult(token_iterator);

    Token token = *token_iterator;
    if (token.type == TokenType::LEFT_SHIFT || token.type == TokenType::RIGHT_SHIFT) {
        token_iterator++;
        return new BinaryAddNode{
            .lhs = lhs.value(),
            .rhs = parse_binary_add(token_iterator).value(),
            .op = token
        };
    }

    return new BinaryAddNode{
        .lhs = lhs.value(),
        .rhs = nullptr,
        .op = Token{}
    };
}


std::expected<BinaryShiftNode*, ParseError> parse_binary_shift(
    std::vector<Token>::const_iterator & token_iterator
) {
    auto lhs = parse_binary_add(token_iterator);

    Token token = *token_iterator;
    if (token.type == TokenType::LEFT_SHIFT || token.type == TokenType::RIGHT_SHIFT) {
        token_iterator++;
        return new BinaryShiftNode{
            .lhs = lhs.value(),
            .rhs = parse_binary_shift(token_iterator).value(),
            .op = token
        };
    }

    return new BinaryShiftNode{
        .lhs = lhs.value(),
        .rhs = nullptr,
        .op = Token{}
    };
}


std::expected<BinaryBinaryNode*, ParseError> parse_binary_binary(
    std::vector<Token>::const_iterator & token_iterator
) {
    auto lhs = parse_binary_shift(token_iterator);

    Token token = *token_iterator;
    if (token.type == TokenType::BIT_OR || token.type == TokenType::BIT_AND || token.type == TokenType::BIT_XOR) {
        token_iterator++;
        return new BinaryBinaryNode{
            .lhs = lhs.value(),
            .rhs = parse_binary_binary(token_iterator).value(),
            .op = token
        };
    }

    return new BinaryBinaryNode{
        .lhs = lhs.value(),
        .rhs = nullptr,
        .op = Token{}
    };
}


std::expected<ComparisonNode*, ParseError> parse_comparison(
    std::vector<Token>::const_iterator & token_iterator
) {
    auto lhs = parse_binary_binary(token_iterator);

    Token token = *token_iterator;
    if (token.type == TokenType::EQUAL || token.type == TokenType::NOT_EQUAL || token.type == TokenType::GREATER || token.type == TokenType::LESSER || token.type == TokenType::GREATER_EQUAL || token.type == TokenType::LESSER_EQUAL) {
        token_iterator++;
        return new ComparisonNode{
            .lhs = lhs.value(),
            .rhs = parse_comparison(token_iterator).value(),
            .op = token
        };
    }

    return new ComparisonNode{
        .lhs = lhs.value(),
        .rhs = nullptr,
        .op = Token{}
    };
}


std::expected<LogicalNode*, ParseError> parse_logical(
    std::vector<Token>::const_iterator & token_iterator
) {
    auto lhs = parse_comparison(token_iterator);

    Token token = *token_iterator;
    if (token.type == TokenType::OR || token.type == TokenType::AND || token.type == TokenType::XOR) {
        token_iterator++;
        return new LogicalNode{
            .lhs = lhs.value(),
            .rhs = parse_logical(token_iterator).value(),
            .op = token
        };
    }

    return new LogicalNode{
        .lhs = lhs.value(),
        .rhs = nullptr,
        .op = Token{}
    };
}


std::expected<AssignmentNode*,ParseError> parse_assignment(
    std::vector<Token>::const_iterator & token_iterator
) {
    auto lhs = parse_logical(token_iterator);

    Token op = *token_iterator++;

    switch (op.type) {
        case TokenType::ASSIGNMENT:
        case TokenType::EQUAL_ASSIGNMENT:
        case TokenType::GREATER_ASSIGNMENT:
        case TokenType::LESSER_ASSIGNMENT:
        case TokenType::GREATER_EQUAL_ASSIGNMENT:
        case TokenType::LESSER_EQUAL_ASSIGNMENT:
        case TokenType::MODULUS_ASSIGNMENT:
        case TokenType::BIT_OR_ASSIGNMENT:
        case TokenType::BIT_AND_ASSIGNMENT:
        case TokenType::BIT_XOR_ASSIGNMENT:
        case TokenType::BIT_NOT_ASSIGNMENT:
        case TokenType::REFERENCE_ASSIGNMENT:
        case TokenType::DEREFERENCE_ASSIGNMENT:
        case TokenType::OR_ASSIGNMENT:
        case TokenType::AND_ASSIGNMENT:
        case TokenType::XOR_ASSIGNMENT:
        case TokenType::MINUS_ASSIGNMENT:
        case TokenType::PLUS_ASSIGNMENT:
        case TokenType::TIMES_ASSIGNMENT:
        case TokenType::DIVIDE_ASSIGNMENT:
        case TokenType::INTIGER_DIVIDE_ASSIGNMENT:
        case TokenType::NOT_ASSIGNMENT:
        case TokenType::RIGHT_SHIFT_ASSIGNMENT:
        case TokenType::LEFT_SHIFT_ASSIGNMENT:
            return new AssignmentNode{
                .assignment = AssignmentNode::Assignment{
                    .lhs = lhs.value(),
                    .rhs = parse_logical(token_iterator).value(),
                    .op = op
                },
                .type = AssignmentNode::ASSIGNMENT,
            };

        case TokenType::QUESTION_MARK:
        case TokenType::QUESTION_MARK_ASSIGNMENT:
            return new AssignmentNode{
                .ternary = AssignmentNode::Ternary{
                    .condition = lhs.value(),
                    .true_opt = parse_logical(token_iterator).value(),
                    .false_opt = (token_iterator++, parse_logical(token_iterator).value()),
                    .op = op
                },
                .type = AssignmentNode::TERNARY,
            };


        default:
            token_iterator--;
            return new AssignmentNode{
                .nop = lhs.value(),
                .type = AssignmentNode::NOP
            };
    }
}


std::expected<ExpressionNode*,ParseError> parse_expression (
    std::vector<Token>::const_iterator & token_iterator
) {
    return new ExpressionNode {
        .node = parse_assignment(token_iterator).value()
    };
}


ExpressionNode parse (
    const std::vector<Token> & tokens
) {
    auto it = tokens.begin();
    return *parse_expression(it).value();
}