#include "gerb/gerb.hpp"

#include <fstream>
#include <iostream>
#include <variant>
#include <algorithm>
#include "gerb/lexer.hpp"
#include "gerb/parser.hpp"
#include "gerb/compiler.hpp"

Gerb gerb( const std::string & gerb ) {
    auto tokens = tokenize( gerb );
    auto ast_or_error = parse( tokens );
    if ( std::holds_alternative<ParseError>(ast_or_error) ) {
        std::cout
        << "parsing error: "
        << std::get<ParseError>(ast_or_error).error
        << " on line " << std::count( gerb.begin(), std::get<ParseError>(ast_or_error).location->start, '\n' )+1
        << ", character "
        << std::find( std::make_reverse_iterator(std::get<ParseError>(ast_or_error).location->start), std::make_reverse_iterator(gerb.begin()), '\n' ) - std::make_reverse_iterator(std::get<ParseError>(ast_or_error).location->start)
        << std::endl;
    } else {
        std::get<Gerb>(ast_or_error).print( std::cout );
    }

    std::ofstream token_cpp_file{ "out/token.c" };
    std::ofstream token_hpp_file{ "out/token.h" };

    auto ast = std::get<Gerb>(ast_or_error);
    CCode tokens_code = GenerateToken( ast );
    token_cpp_file << tokens_code.code;
    token_hpp_file << tokens_code.header;

    // std::cout << "TOKENS:" << '\n';
    // for ( auto i : tokens )
    //     std::cout << i.capture << '\t' << static_cast<int>(i.type) << std::endl;


    return Gerb{};
}