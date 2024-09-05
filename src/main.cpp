#include "lexer.hpp"
#include "parser/expression.hpp"
#include <string>
#include <iostream>

int main( )
{
    std::string code = "wowzers<===3";
    std::vector<Token> tokens = lex(code);
    tokens.pop_back();
    for (const Token token : tokens) std::cout << '<' << serialize(token.type) << ':' << token_content(token, code) << '>' << '\n';
    std::cout << "\n";

    std::vector<Token>::const_iterator it = tokens.begin();
    ExpressionNode * ast = parse_expression(it).value();
    std::cout << ast->serialize() << std::endl;

    return 0;
}