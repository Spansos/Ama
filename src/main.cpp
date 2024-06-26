#include "gerb/gerb.hpp"
#include <fstream>
#include <sstream>

int main( )
{
    std::ifstream file{ "gerb.gerb" };
    std::stringstream buffer;
    buffer << file.rdbuf();

    gerb( buffer.str() );
}