#include "bound_index_expression_node.h"
#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"

namespace Binding {

BoundIndexExpressionNode::BoundIndexExpressionNode(
    Symbols::VariableSymbol *variable_reference,
    BoundExpressionNode *expression)
    : BoundVariableReferenceExpressionNode(variable_reference),
      m_expression(expression) {}

/*
 * The expression that evaluates to the index of this index expression.
 */
BoundExpressionNode *BoundIndexExpressionNode::expression() const {
  return m_expression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol *BoundIndexExpressionNode::type() const {
  return m_variable_reference->var_type()->as_array_element_type();
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundIndexExpressionNode::kind() const {
  return BoundNodeKind::IndexExpression;
}

} // namespace Binding