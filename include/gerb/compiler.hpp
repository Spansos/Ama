#pragma once

#include <string>
#include "gerb/ast.hpp"

struct CCode {
    std::string header;
    std::string code;
};

CCode GenerateToken( const Gerb & gerb );
CCode GenerateLexer( const Gerb & gerb );
CCode GenerateAST( const Gerb & gerb );
CCode GenerateParser( const Gerb & gerb );