#pragma once

#include <vector>
#include <expected>

#include "parser/error.hpp"
#include "token.hpp"
#include "ast.hpp"

std::expected<StatementNode*,ParseError> parse_statement (
    std::vector<Token>::const_iterator & token_iterator
);