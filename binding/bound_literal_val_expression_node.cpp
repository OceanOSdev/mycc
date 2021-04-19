#include "bound_literal_val_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundLiteralValExpressionNode::BoundLiteralValExpressionNode(int value) :
    m_value(value), m_type((&Symbols::TypeSymbol::Int)->as_const_type()) {}

BoundLiteralValExpressionNode::BoundLiteralValExpressionNode(float value) :
    m_value(value), m_type((&Symbols::TypeSymbol::Float)->as_const_type()) {}

BoundLiteralValExpressionNode::BoundLiteralValExpressionNode(char value) :
    m_value(value), m_type((&Symbols::TypeSymbol::Char)->as_const_type()) {}

BoundLiteralValExpressionNode::BoundLiteralValExpressionNode(std::string value) :
    m_value(value), m_type((&Symbols::TypeSymbol::String)->as_const_type()) {}

/*
 * The int value (if the type stored is an int).
 */
int BoundLiteralValExpressionNode::int_value() const {    
    return std::get<int>(m_value);
}

/*
 * The float value (if the type stored is a float).
 */
float BoundLiteralValExpressionNode::float_value() const {    
    return std::get<float>(m_value);
}

/*
 * The string value (if the type stored is a string).
 */
std::string BoundLiteralValExpressionNode::string_value() const {    
    return std::get<std::string>(m_value);
}

/*
 * The char value (if the type stored is a char).
 */
char BoundLiteralValExpressionNode::char_value() const {    
    return std::get<char>(m_value);
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundLiteralValExpressionNode::type() const {
    return m_type;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundLiteralValExpressionNode::kind() const {
    return BoundNodeKind::LiteralExpression;
}


}