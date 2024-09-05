#pragma once

#include <vector>
#include <expected>

#include "parser/error.hpp"
#include "parser/expression.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "ast.hpp"

std::expected<ExpressionNode*,ParseError> parse_statement (
    std::vector<Token>::const_iterator & token_iterator
);