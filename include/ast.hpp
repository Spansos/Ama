#pragma once

#include <vector>
#include "token.hpp"

struct ExpressionNode;

struct Primary {
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
    UnaryPrefix * lhs;
    BinaryMultNode * rhs;
    Token op;
};

struct BinaryAddNode {
    BinaryMultNode * lhs;
    BinaryAddNode * rhs;
    Token op;
};

struct BinaryShiftNode {
    BinaryAddNode * lhs;
    BinaryShiftNode * rhs;
    Token op;
};

struct BinaryBinaryNode {
    BinaryShiftNode * lhs;
    BinaryBinaryNode * rhs;
    Token op;
};

struct ComparisonNode {
    BinaryBinaryNode * lhs;
    ComparisonNode * rhs;
    Token op;
};

struct LogicalNode {
    ComparisonNode * lhs;
    LogicalNode * rhs;
    Token op;
};

struct AssignmentNode {
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
    AssignmentNode * node;
};