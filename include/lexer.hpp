#pragma once

#include <vector>
#include <string>
#include "token.hpp"

std::string token_content(const Token token, const std::string & code);

std::vector<Token> lex (
    const std::string & code
);