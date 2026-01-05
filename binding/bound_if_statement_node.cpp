#include "bound_if_statement_node.h"
#include "bound_expression_node.h"

namespace Binding {

BoundIfStatementNode::BoundIfStatementNode(BoundExpressionNode *condition,
                                           BoundStatementNode *then_stmt,
                                           BoundStatementNode *else_stmt)
    : m_condition(condition), m_then_statement(then_stmt),
      m_else_statement(else_stmt) {}

BoundExpressionNode *BoundIfStatementNode::condition() const {
  return m_condition;
}

BoundStatementNode *BoundIfStatementNode::then_statement() const {
  return m_then_statement;
}

BoundStatementNode *BoundIfStatementNode::else_statement() const {
  return m_else_statement;
}

bool BoundIfStatementNode::has_else_statement() const {
  return m_else_statement != nullptr;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundIfStatementNode::kind() const {
  return BoundNodeKind::IfStatement;
}

} // namespace Binding