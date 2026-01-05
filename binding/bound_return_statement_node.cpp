#include "bound_return_statement_node.h"
#include "bound_expression_node.h"

namespace Binding {

BoundReturnStatementNode::BoundReturnStatementNode(
    BoundExpressionNode *expression)
    : m_expression(expression) {}

BoundExpressionNode *BoundReturnStatementNode::expression() const {
  return m_expression;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundReturnStatementNode::kind() const {
  return BoundNodeKind::ReturnStatement;
}

} // namespace Binding