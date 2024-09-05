#include "ast.hpp"

#include <sstream>

std::string indent(std::string string, int space_count) {
    std::string r;
    r.append(space_count, ' ');
    for ( auto it = string.begin(); it != string.end(); ++it ) {
        r += *it;
        if (*it == '\n' && &*it != &string.back())
            r.append(space_count, ' ');
    }
    return r;
}

std::string serialize_token(Token token) {
    std::stringstream r;
    r << '<' << serialize(token.type) << '@' << token.start << '>';
    return r.str();
}

std::string Primary::serialize() {
    std::stringstream r;
    switch (type) {
        case LITERAL:
            r << serialize_token(literal);
            break;
        case EXPRESSION:
            r << expression->serialize();
            break;
    }
    return r.str();
}

std::string UnaryPostfix::serialize() {
    std::stringstream r;
    switch (type) {
        case FUNCTION:
            r << "{\n" << indent(function.node->serialize(), 2) << "\n,";
            for (const auto & i : function.arguments)
                r << indent(i->serialize(), 2) << "\n,";
            r << "}";
            break;
        case INDEX:
            r << "{\n" << indent(index.node->serialize(), 2) << "\n,";
            r << indent(index.node->serialize(), 2) << "\n}";
            break;
        case NOP:
            r << nop->serialize();
            break;
    }
    return r.str();
}

std::string UnaryPrefix::serialize() {
    std::stringstream r;
    switch (type) {
        case PREFIX:
            r << "{\n" << indent(prefix.node->serialize(), 2) << ",\n" << indent(serialize_token(prefix.op), 2) << "\n}";
            break;
        case NOP:
            r << nop->serialize();
            break;
    }
    return r.str();
}

std::string BinaryMultNode::serialize() {
    if (!rhs)
        return lhs->serialize();
    return "{\n" + indent(lhs->serialize(), 2) + ",\n" + indent(serialize_token(op), 2) + ",\n" + indent(rhs->serialize(), 2) + "\n}";
}

std::string BinaryAddNode::serialize() {
    if (rhs == nullptr)
        return lhs->serialize();
    return "{\n" + indent(lhs->serialize(), 2) + ",\n" + indent(serialize_token(op), 2) + ",\n" + indent(rhs->serialize(), 2) + "\n}";
}

std::string BinaryShiftNode::serialize()  {
    if (!rhs)
        return lhs->serialize();
    return "{\n" + indent(lhs->serialize(), 2) + ",\n" + indent(serialize_token(op), 2) + ",\n" + indent(rhs->serialize(), 2) + "\n}";
}

std::string BinaryBinaryNode::serialize()  {
    if (!rhs)
        return lhs->serialize();
    return "{\n" + indent(lhs->serialize(), 2) + ",\n" + indent(serialize_token(op), 2) + ",\n" + indent(rhs->serialize(), 2) + "\n}";
}

std::string ComparisonNode::serialize()  {
    if (!rhs)
        return lhs->serialize();
    return "{\n" + indent(lhs->serialize(), 2) + ",\n" + indent(serialize_token(op), 2) + ",\n" + indent(rhs->serialize(), 2) + "\n}";
}

std::string LogicalNode::serialize()  {
    if (!rhs)
        return lhs->serialize();
    return "{\n" + indent(lhs->serialize(), 2) + ",\n" + indent(serialize_token(op), 2) + ",\n" + indent(rhs->serialize(), 2) + "\n}";
}

std::string AssignmentNode::serialize()  {
    switch (type) {
        case TERNARY:
            return "{\n" + indent(ternary.condition->serialize(), 2) + ",\n" + indent(serialize_token(ternary.op), 2) + ",\n" + indent(ternary.false_opt->rhs->serialize(), 2) + ",\n" + indent(ternary.true_opt->rhs->serialize(), 2) + "\n}";
        case ASSIGNMENT:
            return "{\n" + indent(assignment.lhs->serialize(), 2) + ",\n" + indent(serialize_token(assignment.op), 2) + ",\n" + indent(assignment.rhs->serialize(), 2) + "\n}";
        case NOP:
            return nop->serialize();
    }
    return "";
}

std::string ExpressionNode::serialize() {
    return node->serialize();
}