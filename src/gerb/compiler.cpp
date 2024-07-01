#include "gerb/compiler.hpp"

#include <iostream>


std::string capture_to_enum( std::string capture ) {
    const int max_type_hash = 1'000'000;
    std::hash<std::string> hasher;
    return "type_" + std::to_string( hasher( capture ) % max_type_hash );
};

std::string sanitize( std::string string ) {
    std::string r;
    for ( const auto & c : string ) {
        if ( c == '\\' )
            r += c;
        r += c;
    }
    return r;
}

CCode GenerateToken( const Gerb & gerb ) {
    (void)gerb;

    CCode code;

    code.header += "#pragma once\n\n";
    code.header += "#include <stddef.h>\n\n";

    // token types
    code.header += "enum token_type {\n";
    for ( const auto & term : gerb.terminals )
        code.header += "    " + capture_to_enum( term.terminal.capture ) + ",\n";
    code.header += "};\n\n";

    // token
    code.header +=
    "struct token {\n"
    "    enum token_type type;\n"
    "    size_t start;\n"
    "    size_t length;\n"
    "};\n\n";

    // token to string
    code.header += "const char * token_type_to_string( enum token_type );";

    code.code += "#include \"token.h\"\n\n";
    code.code += "const char * token_type_to_string( enum token_type type ) {\n";
    code.code += "    switch ( type ) {\n";
    for ( const auto & term : gerb.terminals ) {
        code.code += "    case " + capture_to_enum( term.terminal.capture ) + ":\n";
        code.code += "        return \"" + sanitize( term.terminal.capture ) + "\";\n";
    }
    code.code += "    }\n}";

// struct Token {
//     TokenType type;
//     std::string capture;
//     std::string::const_iterator start;
// };

    return code;
}

CCode GenerateLexer( const Gerb & gerb ) {
    (void)gerb;
    return {};
}

CCode GenerateAST( const Gerb & gerb ) {
    (void)gerb;
    return {};
}

CCode GenerateParser( const Gerb & gerb ) {
    (void)gerb;
    return {};
}