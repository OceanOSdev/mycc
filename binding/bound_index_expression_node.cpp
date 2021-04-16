#include "bound_index_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundIndexExpressionNode::BoundIndexExpressionNode(Symbols::VariableSymbol* variable_reference, BoundExpressionNode* expression) :
    BoundVariableReferenceExpressionNode(variable_reference), m_expression(expression) {}


/*
 * The expression that evaluates to the index of this index expression.
 */
BoundExpressionNode* BoundIndexExpressionNode::expression() const {
    return m_expression;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundIndexExpressionNode::kind() const {
    return BoundNodeKind::IndexExpression;
}

}