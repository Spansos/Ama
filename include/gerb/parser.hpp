#pragma once

#include <variant>
#include <vector>
#include "gerb/token.hpp"
#include "gerb/ast.hpp"

struct ParseError {
    std::string error;
    std::vector<Token>::const_iterator location;
};

std::variant<Gerb, ParseError> parse( const std::vector<Token> & tokens );