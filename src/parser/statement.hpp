#pragma once

#include "parser/statement.hpp"

std::expected<ExpressionNode*,ParseError> parse_statement (
    std::vector<Token>::const_iterator & token_iterator
) {
    
}