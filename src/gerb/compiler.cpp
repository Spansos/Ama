#include "gerb/compiler.hpp"

#include <iostream>
#include <unordered_set>
#include <limits>

std::string capture_to_enum( std::string capture ) {
    const int max_type_hash = 1'000'000;
    std::hash<std::string> hasher;
    return "type_" + std::to_string( hasher( capture ) % max_type_hash );
}


std::string escape_characters( std::string string ) {
    std::string r;
    for ( const auto & c : string ) {
        if ( c == '\'' || c == '"' ) {
            r += '\\';
            r += c;
        } else if ( c == '\n' )
            r += "\\n";
        else if ( c == '\0' )
            r += "\\0";
        else if ( c == '\t' )
            r += "\\t";
        else
            r += c;
    }
    return r;
}


std::string interpret_string( std::string string ) {
    std::string r;
    for ( auto it = string.begin(); it != string.end(); it++ ) {
        if ( *it == '\\' ) {
            switch ( *(++it) ) {
            case 'n':
                r += '\n';
                break;
            case 't':
                r += '\t';
                break;
            case '0':
                r += '\0';
                break;
            default:
                r += *it;
                break;
            }
        } else
            r += *it;
    }
    return r;
}


std::unordered_set<char> terminal_start_chars( const TerminalExpr & expr ) {
    std::unordered_set<char> r;
    // go over all children of first child
    for ( const auto & postfix : expr.children[0].children ) {
        // if any has postfix (= is optional), error
        if ( postfix.op.has_value() && postfix.op.value().type != TokenType::PLUS ) {
            throw std::runtime_error( "First character may not be optional." );
        }
        const auto & primary = postfix.child.child;
        // if primary is a token
        if ( std::holds_alternative<Token>( primary ) ) {
            const Token & token = std::get<Token>( primary );
            switch ( token.type ) {
            // if token is '.'
            case TokenType::DOT:
                for ( char c = 0; c <= std::numeric_limits<char>::max(); c++ )
                    r.insert( c );
                break;
            // if token is an exact match
            case TokenType::EXACT:
                for ( char c : interpret_string( std::string( token.capture.substr( 1, token.capture.length()-2 ) ) ) )
                    r.insert( c );
                break;
            // if token is a union
            case TokenType::UNION:
                for ( char c : interpret_string( token.capture.substr( 1, token.capture.length()-2 ) ) )
                    r.insert( c );
                break;
            // other, just error
            default:
                throw std::runtime_error( "Token was not token" );
            }
        // if primary is a expression
        } else {
            r = terminal_start_chars( *std::get<TerminalExpr*>( primary ) );
        }
    }
    return r;
}


CCode CompileToken( const Gerb & gerb ) {
    CCode code;

    // includes
    code.header += "#pragma once\n\n";
    code.header += "#include <stddef.h>\n\n";

    code.code += "#include \"token.h\"\n\n";
    code.code += "#include <stdlib.h>\n";
    code.code += "#include <assert.h>\n\n";

    // data types
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

    // token string
    code.header +=
        "    struct token_string {\n"
        "        struct token * tokens;\n"
        "        size_t length;\n"
        "        size_t size;\n"
        "    };\n\n";

    // functionss
    // token to string
    code.header += "const char * token_type_to_string( enum token_type );\n";

    code.code += "const char * token_type_to_string( enum token_type type ) {\n";
    code.code += "    switch ( type ) {\n";
    for ( const auto & term : gerb.terminals ) {
        code.code += "    case " + capture_to_enum( term.terminal.capture ) + ":\n";
        code.code += "        return \"" + escape_characters( term.terminal.capture ) + "\";\n";
    }
    code.code += "    default:\n";
    code.code += "        return NULL;\n";
    code.code += "    }\n}\n\n";

    // token_string initialization
    code.header += "struct token_string token_string_init( );\n";
    code.code +=
        "struct token_string token_string_init( ) {\n"
        "    struct token_string string = { .tokens=malloc( sizeof(struct token) ), .length=0, .size=1 };\n"
        "    assert( string.tokens != NULL );\n"
        "    return string;\n"
        "}\n\n";

    // token_string appendation
    code.header += "void token_string_append( struct token_string * string, struct token token );\n";
    code.code +=
        "void token_string_append( struct token_string * string, struct token token ) {\n"
        "    if ( ++string->length > string->size ) {\n"
        "        string->size *= 2;\n"
        "        string->tokens = realloc( string->tokens, string->size * sizeof( token ) );\n"
        "        assert( string->tokens != NULL );\n"
        "    }\n"
        "    string->tokens[ string->length - 1 ] = token;\n"
        "}";

    return code;
}


CCode CompileLexer( const Gerb & gerb ) {
    CCode code;

    // includes
    code.header += "#pragma once\n\n";
    code.header += "#include <stddef.h>\n";
    code.header += "#include <stdbool.h>\n";
    code.header += "#include \"token.h\"\n\n";
    code.code += "#include \"lexer.h\"\n\n";
    
    // datastructures
    // lex_error
    code.header +=
        "struct lex_error {\n"
        "    enum token_type expected;\n"
        "    size_t token_start;\n"
        "    size_t location;\n"
        "};\n\n";

    // lex_output
    code.header +=
        "struct lex_output {\n"
        "    union {\n"
        "        struct token_string output;\n"
        "        struct lex_error error;\n"
        "    };\n"
        "    bool is_error;\n"
        "};\n\n";
    
    // functions
    // lex
    code.header += "struct lex_output lex( char * code );";

    code.code += "struct lex_output lex( char * code ) {\n";
    code.code += "    switch ( *code ) {\n";

    for ( const auto & term : gerb.terminals ) {
        for ( char c : terminal_start_chars( term.expression ) ) {
            code.code += "case '" + escape_characters( std::string{c} ) + "': ";
            // std::cout << "orig: <" << c << "> escaped: <" << escape_characters( std::string{c} ) << '>'  <<std::endl;
        }
        code.code += "\n";
    }

    return code;
}

CCode CompileAST( const Gerb & gerb ) {
    (void)gerb;
    return {};
}

CCode CompileParser( const Gerb & gerb ) {
    (void)gerb;
    return {};
}