#pragma once

#include <vector>
#include "token.hpp"

std::vector<Token> lex (
    const std::string & code
);