#pragma once

#include <vector>
#include <variant>
#include <string>
#include <token.hpp>

class NonTerminalType
{
public:
private:
    std::string name;
};


class NonTerminal
{
public:
private:
    NonTerminalType & type;
};


class Rule
{
public:
private:
    NonTerminalType & lhs;
    std::vector< std::variant<TokenType, NonTerminalType> > rhs;  
};


class Grammar
{
    // terminals
    std::vector<TokenType> terminals;
    // non-terminals
    std::vector<NonTerminalType> nonTerminals;
    // rules
    std::vector<Rule> rules;
    // start non-terminal
    NonTerminal & start;
};