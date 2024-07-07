#include "gerb/compiler.hpp"

#include <iostream>
#include <unordered_set>
#include <sstream>
#include <limits>

std::string capture_to_enum( std::string capture ) {
    const int max_type_hash = 1'000'000;
    std::hash<std::string> hasher;
    return "type_" + std::to_string( hasher( capture ) % max_type_hash );
}

std::string escape_characters( std::string string ) {
    std::stringstream r;
    for ( int c : string ) {
        r << "\\x" << std::hex << c;
    }
    return r.str();
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

std::string indent( std::string string, int space_count ) {
    std::string r = "    ";
    for ( auto it = string.begin(); it != string.end(); ++it ) {
        r += *it;
        if ( *it == '\n' && &*it != &string.back() )
            r.append( space_count, ' ' );
    }
    return r;
}

CCode compile_token( const Gerb & gerb ) {
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
        "struct token_string {\n"
        "    struct token * tokens;\n"
        "    size_t length;\n"
        "    size_t size;\n"
        "};\n\n";

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

std::unordered_set<char> terminal_expr_start_chars( const TerminalExpr & terminal_expr );

std::unordered_set<char> terminal_primary_start_chars( const TerminalPrimary & terminal_primary ) {
    if ( std::holds_alternative<TerminalExpr*>( terminal_primary ) )
        return terminal_expr_start_chars( *std::get<TerminalExpr*>( terminal_primary ) );

    std::unordered_set<char> r;
    const Token & token = std::get<Token>( terminal_primary );
    switch ( token.type ) {
        case TokenType::DOT:
            for ( int i=std::numeric_limits<char>::min(); i<=std::numeric_limits<char>::max(); i++ ) {
                r.insert( i );
            }
            break;
        case TokenType::EXACT:
            r.insert( interpret_string( std::string( token.capture.substr( 1, token.capture.length()-2 ) ) )[0] );
            break;
        case TokenType::UNION:
            for ( char c : interpret_string( token.capture.substr( 1, token.capture.length()-2 ) ) )
                r.insert( c );
            break;
        default:
            throw std::runtime_error( "Token was not token" );
    }
    return r;
}

std::unordered_set<char> terminal_prefix_start_chars( const TerminalPrefix & terminal_prefix ) {
    auto r = terminal_primary_start_chars( terminal_prefix.child );
    if ( terminal_prefix.op.has_value( ) && terminal_prefix.op.value( ).capture == "!" ) {
        for ( int i=std::numeric_limits<char>::min(); i<=std::numeric_limits<char>::max(); i++ ) {
            if ( r.count( i ) ) {
                r.erase( i );
            } else {
                r.insert( i );
            }
        }
    }
    return r;
                
}

std::unordered_set<char> terminal_postfix_start_chars( const TerminalPostfix & terminal_postfix ) {
    if ( terminal_postfix.op.has_value( ) && terminal_postfix.op.value().type != TokenType::PLUS )
        throw std::runtime_error( "possible first chars of an option shouldn't be optional" );
    return terminal_prefix_start_chars( terminal_postfix.child );
}

std::unordered_set<char> terminal_or_start_chars( const TerminalOr & terminal_or ) {
    std::unordered_set<char> r;
    for ( const auto & child : terminal_or.children )
        r.merge( terminal_postfix_start_chars( child ) );
    return r;
}

std::unordered_set<char> terminal_expr_start_chars( const TerminalExpr & terminal_expr ) {
    return terminal_or_start_chars( terminal_expr.children[0] );
}

std::string compile_terminal_prefix( const TerminalPrefix & terminal_prefix ) {
    std::string code = "if ( ";
    auto start_chars = terminal_prefix_start_chars( terminal_prefix );
    auto it = start_chars.begin( );
    code += "code[cur] == '" + escape_characters( std::string{*it++} ) + "' ";
    // auto start_chars.;
    for ( ; it != start_chars.end(); it ++ )
        code += "|| code[cur] == '" + escape_characters( std::string{*it} ) + "' ";
    code += ") token.length++;\n";
    return code;
}

std::string compile_terminal_postfix( const TerminalPostfix & terminal_postfix ) {
    std::string code;
    if ( !terminal_postfix.op )
        return compile_terminal_prefix( terminal_postfix.child );
    switch ( terminal_postfix.op.value().type ) {
        case TokenType::PLUS:
            code += "while ( true ) {\n";
            code += indent( compile_terminal_prefix( terminal_postfix.child ), 4 );
            code += "}\n";
            break;
        case TokenType::STAR:
            code += "while ( true ) {\n";
            code += indent( compile_terminal_prefix( terminal_postfix.child ), 4 );
            code += "}\n";
            break;
        case TokenType::QUESTION_MARK:
            code += compile_terminal_prefix( terminal_postfix.child );
            break;
        default:
            throw std::runtime_error( "Token not token" );
    }
    return code;
}

std::string compile_terminal_or( const TerminalOr & terminal_or ) {
    std::string code;
    code += "switch ( code[cur] ) {\n";
    for ( const auto & terminal_postfix : terminal_or.children ) {
        for ( char c : terminal_postfix_start_chars( terminal_postfix ) )
            code += "case '" + escape_characters( std::string{c} ) + "': ";
        code += "\n";
        code += indent( compile_terminal_postfix( terminal_postfix ), 4 );
        code += "    break;\n";
    }
    code += "}\n";
    return code;
}

std::string compile_terminal_expr( const TerminalExpr & terminal_expr ) {
    std::string code;
    for ( const auto & terminal_or : terminal_expr.children ) {
        code += "{\n";
        code += indent( compile_terminal_or( terminal_or ), 4 );
        code += "}\n";
    }
    return code;
}

std::string compile_terminal( const Terminal & terminal ) {
    std::string code;
    for ( char c : terminal_expr_start_chars( terminal.expression ) )
        code += "case '" + escape_characters( std::string{c} ) + "': ";
    code += "{\n";
    code += "    token.type = " + capture_to_enum( terminal.terminal.capture ) + ";\n";
    code += indent( compile_terminal_expr( terminal.expression ), 4 );
    code += "    break;\n";
    code += "}\n";
    return code;
}

std::string compile_terminals( const std::vector<Terminal> & terminals ) {
    std::string code =
        "size_t cur = 0;\n"
        "while ( code[cur] != '\\0' ) {\n"
        "    struct token token = (struct token){ .type=-1, .start=cur, .length=0 };\n"
        "    switch ( code[cur] ) {\n";

    for ( const auto & terminal : terminals )
        code += indent( compile_terminal( terminal ), 4 );


    code +=
        "    error:\n"
        "    default:\n"
        "        token.length = 0;\n"
        "    }\n"
        "}\n"
        "if ( token.length == 0 ) {\n"
        "    free( out.output.tokens );\n"
        "    return ( struct lex_output ){ .error=( struct lex_error ){ .expected=token.type, .location=token.start }, .is_error=true };\n"
        "}\n"
        "cur += token.length;\n"
        "token_string_append( &out.output, token );\n";
    
    return code;
}

CCode compile_lexer( const Gerb & gerb ) {
    CCode code;

    // includes
    code.header += "#pragma once\n\n";
    code.header += "#include <stddef.h>\n";
    code.header += "#include <stdbool.h>\n";
    code.header += "#include \"token.h\"\n\n";
    code.code += "#include \"lexer.h\"\n\n";
    code.code += "#include <stdlib.h>\n\n";
    
    // datastructures
    // lex_error
    code.header +=
        "struct lex_error {\n"
        "    enum token_type expected;\n"
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
    code.code += "    struct lex_output out = ( struct lex_output ){ .output=token_string_init( ), .is_error=false };\n";
    code.code += indent( compile_terminals( gerb.terminals ), 4 );
    code.code += "    return out;\n";
    code.code += "}\n";

    return code;
}

CCode compile_ast( const Gerb & gerb ) {
    (void)gerb;
    return {};
}

CCode compile_parser( const Gerb & gerb ) {
    (void)gerb;
    return {};
}