#include "parser.hpp"

#include <expected>

enum class ParseError {

};

std::expected<LogicalNode*, ParseError> parse_logical(
    std::vector<Token>::const_iterator & token_iterator
) {
    return {};
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
        case TokenType::QUESTION_MARK_ASSIGNMENT:
        case TokenType::RIGHT_SHIFT_ASSIGNMENT:
        case TokenType::LEFT_SHIFT_ASSIGNMENT:
        case TokenType::BINARY_OR_ASSIGNMENT:
        case TokenType::BINARY_AND_ASSIGNMENT:
        case TokenType::BINARY_XOR_ASSIGNMENT:
        case TokenType::BINARY_NOT_ASSIGNMENT:
            return new AssignmentNode{
                .assignment = AssignmentNode::Assignment{
                    .lhs = lhs.value(),
                    .rhs = parse_logical(token_iterator).value(),
                    .op = op
                },
                .type = AssignmentNode::ASSIGNMENT,
            };
        default:
            return std::unexpected{ParseError{}};
    }
}



std::expected<ExpressionNode*,ParseError> parse_expression (
    std::vector<Token>::const_iterator & token_iterator
) {
    return new ExpressionNode {
        .node = parse_assignment(token_iterator).value();
    }
}

ExpressionNode parse (
    const std::vector<Token> & token
) {
    auto it = tokens.begin();
    return *parse_expression(it).value();
}