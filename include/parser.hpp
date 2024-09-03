#pragma once

#include <vector>
#include <lexer.hpp>
#include "ast.hpp"
#include "token.hpp"

ExpressionNode parse (
    const std::vector<Token> & tokens
);