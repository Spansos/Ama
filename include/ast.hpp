#pragma once

#include <vector>
#include <iostream>
#include "token.hpp"

struct ExpressionNode;

struct Primary {
    std::string serialize();
    union {
        Token literal;
        ExpressionNode * expression;
    };
    enum {
        LITERAL,
        EXPRESSION
    } type;
};

struct UnaryPostfix {
    std::string serialize();
    struct Function {
        Primary * node;
        std::vector<ExpressionNode*> arguments;
    };
    struct Index {
        Primary * node;
        ExpressionNode * index;
    };

    union {
        Function function;
        Index index;
        Primary * nop;
    };

    enum {
        FUNCTION,
        INDEX,
        NOP
    } type;
};

struct UnaryPrefix {
    std::string serialize();
    struct Prefix {
        UnaryPrefix * node;
        Token op;
    };
    
    union {
        Prefix prefix;
        UnaryPostfix * nop;
    };

    enum {
        PREFIX,
        NOP
    } type;
};

struct BinaryMultNode {
    std::string serialize();
    UnaryPrefix * lhs;
    BinaryMultNode * rhs;
    Token op;
};

struct BinaryAddNode {
    std::string serialize();
    BinaryMultNode * lhs;
    BinaryAddNode * rhs;
    Token op;
};

struct BinaryShiftNode {
    std::string serialize();
    BinaryAddNode * lhs;
    BinaryShiftNode * rhs;
    Token op;
};

struct BinaryBinaryNode {
    std::string serialize();
    BinaryShiftNode * lhs;
    BinaryBinaryNode * rhs;
    Token op;
};

struct ComparisonNode {
    std::string serialize();
    BinaryBinaryNode * lhs;
    ComparisonNode * rhs;
    Token op;
};

struct LogicalNode {
    std::string serialize();
    ComparisonNode * lhs;
    LogicalNode * rhs;
    Token op;
};

struct AssignmentNode {
    std::string serialize();
    struct Ternary {
        LogicalNode * condition;
        LogicalNode * true_opt;
        LogicalNode * false_opt;
        Token op;
    };
    struct Assignment {
        LogicalNode * lhs;
        LogicalNode * rhs;
        Token op;
    };

    union {
        Ternary ternary;
        Assignment assignment;
        LogicalNode * nop;
    };

    enum {
        TERNARY,
        ASSIGNMENT,
        NOP
    } type;
};

struct ExpressionNode {
    std::string serialize();
    AssignmentNode * node;
};