#pragma once

#include <string>
#include "gerb/ast.hpp"

struct CCode {
    std::string header;
    std::string code;
};

CCode compile_token( const Gerb & gerb );
CCode compile_lexer( const Gerb & gerb );
CCode compile_ast( const Gerb & gerb );
CCode compile_parser( const Gerb & gerb );