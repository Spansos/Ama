#include "lexer.hpp"
#include <string>
#include <iostream>

int main( )
{
    std::string code = "u16 i = 20;\n u16 * ip = @i;\n## comment ##\nip <=== i x2;";
    std::vector<Token> tokens = lex(code);
    for (const Token token : tokens) {
        std::cout << '<' << (int)token.type << ':' << token_content(token, code) << '>' << '\n';
    }
    return 0;
}