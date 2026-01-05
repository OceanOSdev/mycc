#include "bound_cast_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundCastExpressionNode::BoundCastExpressionNode(
    const Symbols::TypeSymbol *cast_type, BoundExpressionNode *expression)
    : m_cast_type(cast_type), m_expression(expression) {}

BoundExpressionNode *BoundCastExpressionNode::expression() const {
  return m_expression;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundCastExpressionNode::kind() const {
  return BoundNodeKind::CastExpression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol *BoundCastExpressionNode::type() const {
  return m_cast_type;
}

} // namespace Binding