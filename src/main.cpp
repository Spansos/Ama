#include "lexer.hpp"
#include "parser.hpp"
#include <string>
#include <iostream>

int main( )
{
    std::string code = "wowzers<===3";
    std::vector<Token> tokens = lex(code);
    tokens.pop_back();
    for (const Token token : tokens) std::cout << '<' << (int)token.type << ':' << token_content(token, code) << '>' << '\n';

    ExpressionNode ast = parse(tokens);
    std::cout << ast.serialize() << std::endl;

    return 0;
}