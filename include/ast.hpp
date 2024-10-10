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

struct StatementNode;

struct BlockNode {
    std::vector<StatementNode*> statements;
};

// type        = 'var'? ( ( '&'|'*' type ) | IDENTIFIER )
struct TypeNode {
    bool var;

    union {
        TypeNode * pointer;
        TypeNode * reference;
        Token identifier;
    };

    enum {
        POINTER,
        REFERENCE,
        IDENTIFIER
    } type;
};

struct DeclarationNode;

// enum-definition = 'enum' IDENTIFIER '{' (type? IDENTIFIER ';')* '}'
struct EnumNode {
    Token identifier;
    std::vector<DeclarationNode*> members;
};

// struct-definition = 'struct' IDENTIFIER '{' (variable-declaration | struct-defintion | enum-definition ';')* '}'
struct StructNode {
    Token identifier;
    std::vector<DeclarationNode*> members;
};

// variable-declaration = type IDENTIFIER ('=' expression)?
struct VarDeclNode {
    Token identifier;
    TypeNode * type;
    ExpressionNode * value;
};

// func-definition = type IDENTIFIER '(' arguments ')' 'var'? statement
struct FuncNode {
    TypeNode return_type;
    Token identifier;
    std::vector<VarDeclNode*> parameters;
    StatementNode * body;
};

struct DeclarationNode {
    union {
        StructNode * struct_decl;
        FuncNode * func_decl;
        VarDeclNode * var_decl;
    };
    enum {
        STRUCT_DECL,
        FUNC_DECL,
        VAR_DECL
    } type;
};

// do-while    = 'do' statement 'while' '(' expression ')'
struct DoWhileNode {
    ExpressionNode * condition;
    StatementNode * body;
};

// while       = 'while' '(' expression ')' statement
struct WhileNode {
    ExpressionNode * condition;
    StatementNode * body;
};

// for         = 'for' '(' statement expression ';' statement ')' statement
struct ForNode {
    StatementNode * start;
    ExpressionNode * condition;
    StatementNode * update;
    StatementNode * body;
};

// ( (variable-declaration | expression)? ';' ) | for | while | do-while | func-definition | struct-definition | enum-definition | code-block
struct StatementNode {
    union {
        VarDeclNode * var_decl;
        ExpressionNode * expression;
        ForNode * for_loop;
        WhileNode * while_loop;
        DoWhileNode * do_while_loop;
        FuncNode * func;
        StructNode * struct_decl;
        EnumNode * enum_decl;
        BlockNode * code_block;
    };
    enum {
        VAR_DECL,
        EXPRESSION,
        FOR_LOOP,
        WHILE_LOOP,
        DO_WHILE_LOOP,
        FUNC,
        STRUCT_DECL,
        ENUM_DECL,
        CODE_BLOCK
    } type;
};