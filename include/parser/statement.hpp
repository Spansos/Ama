#pragma once

#include "parser/common.hpp"

std::expected<StatementNode*,ParseError> parse_statement (
    std::vector<Token>::const_iterator & token_iterator
);