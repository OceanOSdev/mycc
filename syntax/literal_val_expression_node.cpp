#include "literal_val_expression_node.h"

namespace Syntax {

LiteralValExpressionNode::LiteralValExpressionNode(LiteralValType type, LiteralValWrapper value) :
    v_type(type), v_wrapper(value) {}

LiteralValExpressionNode::~LiteralValExpressionNode() { }

/*
 * An enum value representing the type stored in this node.
 */
LiteralValType LiteralValExpressionNode::value_type() const { 
    return v_type; 
}

/*
 * The int value (if the type stored is an int).
 */
int LiteralValExpressionNode::int_value() const { 
    return v_wrapper.i_val; 
}

/*
 * The float value (if the type stored is a float).
 */
float LiteralValExpressionNode::float_value() const { 
    return v_wrapper.f_val; 
}

/*
 * The string value (if the type stored is a string).
 */
std::string LiteralValExpressionNode::string_value() const { 
    return v_wrapper.s_val; 
}

/*
 * The char value (if the type stored is a char).
 */
char LiteralValExpressionNode::char_value() const { 
    return v_wrapper.c_val; 
}


}