#include "lexer.hpp"
#include "parser.hpp"
#include <string>
#include <iostream>

int main( )
{
    std::string code = "2*4";
    std::vector<Token> tokens = lex(code);
    tokens.pop_back();
    for (const Token token : tokens) std::cout << '<' << (int)token.type << ':' << token_content(token, code) << '>' << '\n';

    ExpressionNode ast = parse(tokens);
    (void)ast;

    return 0;


}