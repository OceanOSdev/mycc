#include "bound_do_while_statement_node.h"
#include "bound_expression_node.h"
#include "bound_label_statement_node.h"

namespace Binding {

BoundDoWhileStatementNode::BoundDoWhileStatementNode(BoundExpressionNode *cond,
                                                     BoundStatementNode *body,
                                                     BoundLabel *break_lbl,
                                                     BoundLabel *cont_lbl)
    : m_cond_expression(cond), m_body_statement(body), m_break_label(break_lbl),
      m_continue_label(cont_lbl) {}

BoundExpressionNode *BoundDoWhileStatementNode::condition_expression() const {
  return m_cond_expression;
}

BoundStatementNode *BoundDoWhileStatementNode::body_statement() const {
  return m_body_statement;
}

BoundLabel *BoundDoWhileStatementNode::break_label() const {
  return m_break_label;
}

BoundLabel *BoundDoWhileStatementNode::continue_label() const {
  return m_continue_label;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundDoWhileStatementNode::kind() const {
  return BoundNodeKind::DoWhileStatement;
}

} // namespace Binding