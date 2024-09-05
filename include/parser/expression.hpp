#pragma once

#include <vector>
#include <expected>

#include "parser/parse_error.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "ast.hpp"

std::expected<ExpressionNode*,ParseError> parse_expression (
    std::vector<Token>::const_iterator & token_iterator
);