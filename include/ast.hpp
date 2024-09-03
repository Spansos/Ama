#pragma once

#include <vector>
#include <optional>
#include "token.hpp"

struct ExpressionNode;

struct Primary {
    union {
        Token token;
        ExpressionNode * expression;
    };
    enum {
        TOKEN,
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
        Primary nop;
    };

    enum {
        FUNCTION,
        INDEX,
        NOP
    } type;
};

struct BinaryMultNode {
    UnaryPostfix * node;
    std::optional<Token> op;
};

struct BinaryAddNode {
    BinaryAddNode * lhs;
    BinaryMultNode * rhs;
    Token token;
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
        LogicalNode nop;
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