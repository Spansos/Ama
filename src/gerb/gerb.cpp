#include "gerb/gerb.hpp"

#include <iostream>
#include "gerb/lexer.hpp"


Gerb gerb( const std::string & gerb ) {
    auto tokens = tokenize( gerb );

    std::cout << "TOKENS:" << '\n';
    for ( auto i : tokens ) {
        std::cout << i.capture << '\t' << static_cast<int>(i.type) << std::endl;
    }

    return Gerb{};
}