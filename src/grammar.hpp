#pragma once

#include <vector>
#include "token.hpp"

class NonTerminalType
{
private:
    
    std::string name;
};

class NonTerminal
{
private:
    NonTerminalType & type;
};

class Rule
{

};

class Grammar
{
    // non-terminals
    // terminals (tokens)
    const std::vector<TokenType> terminals;
    // rules
    // start non-terminal
    const TokenType & start;
};