#pragma once

#include <vector>
#include <expected>

#include "lexer.hpp"
#include "token.hpp"
#include "ast.hpp"

enum class ParseError {
    INVALID_TYPE

};

std::expected<Token, ParseError> check(
    std::vector<Token>::const_iterator & token_iterator,
    std::initializer_list<TokenType> types
) {
    for (TokenType type : types)
        if (token_iterator->type == type)
            return *token_iterator;
    return std::unexpected{ParseError{}};
}

std::expected<Token, ParseError> match(
    std::vector<Token>::const_iterator begin_iterator,
    std::vector<Token>::const_iterator & token_iterator,
    std::initializer_list<TokenType> types
) {
    auto r = check(token_iterator, types);
    if (r)
        return token_iterator++, r.value();
    token_iterator = begin_iterator;
    return std::unexpected{r.error()};
}