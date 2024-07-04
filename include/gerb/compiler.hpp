#pragma once

#include <string>
#include "gerb/ast.hpp"

struct CCode {
    std::string header;
    std::string code;
};

CCode CompileToken( const Gerb & gerb );
CCode CompileLexer( const Gerb & gerb );
CCode CompileAST( const Gerb & gerb );
CCode CompileParser( const Gerb & gerb );