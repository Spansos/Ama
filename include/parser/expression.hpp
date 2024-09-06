#pragma once

#include "parser/common.hpp"

std::expected<ExpressionNode*,ParseError> parse_expression (
    std::vector<Token>::const_iterator & token_iterator
);