#include "ast.hpp"

std::string Primary::serialize() {
    switch (type) {
        case LITERAL:
            stream << '<' << static_cast<int>(literal.type) << '@' << literal.start << '>';
            break;
        case EXPRESSION:
            expression->serialize(stream);
            break;
    }
    return stream;
}

std::string UnaryPostfix::serialize();
std::string UnaryPrefix::serialize();
std::string BinaryMultNode::serialize();
std::string BinaryAddNode::serialize();
std::string BinaryShiftNode::serialize();
std::string BinaryBinaryNode::serialize();
std::string ComparisonNode::serialize();
std::string LogicalNode::serialize();
std::string AssignmentNode::serialize();
std::string ExpressionNode::serialize();