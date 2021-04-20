#include "bound_increment_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundIncrementExpressionNode::BoundIncrementExpressionNode(IDNotation notation, BoundExpressionNode* expression) :
    m_notation(notation), m_expression(expression) {}

IDNotation BoundIncrementExpressionNode::notation() const {
    return m_notation;
}

BoundExpressionNode* BoundIncrementExpressionNode::expression() const {
    return m_expression;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundIncrementExpressionNode::kind() const {
    return BoundNodeKind::IncrementExpression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundIncrementExpressionNode::type() const {
    return m_expression->type();
}



BoundDecrementExpressionNode::BoundDecrementExpressionNode(IDNotation notation, BoundExpressionNode* expression) :
    m_notation(notation), m_expression(expression) {}

IDNotation BoundDecrementExpressionNode::notation() const {
    return m_notation;
}

BoundExpressionNode* BoundDecrementExpressionNode::expression() const {
    return m_expression;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundDecrementExpressionNode::kind() const {
    return BoundNodeKind::DecrementExpression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundDecrementExpressionNode::type() const {
    return m_expression->type();
}

}