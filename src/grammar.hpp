#pragma once

#include <vector>
#include <variant>
#include <string>

template < class T >
class TerminalType
{
    std::string name;
    T match;
};


class NonTerminal
{
    std::vector< std::variant<NonTerminal, Terminal> > 
};


class Rule
{

};


class Grammar
{
    // terminals
    // non-terminals
    // rules
    // start non-terminal
};