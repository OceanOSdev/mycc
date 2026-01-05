#pragma once
#ifndef BOUND_DO_WHILE_STATEMENT_NODE_H
#define BOUND_DO_WHILE_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {
class BoundLabel;
class BoundExpressionNode;

class BoundDoWhileStatementNode : public BoundStatementNode {
private:
  BoundExpressionNode *m_cond_expression;
  BoundStatementNode *m_body_statement;
  BoundLabel *m_break_label;
  BoundLabel *m_continue_label;

public:
  BoundDoWhileStatementNode(BoundExpressionNode *cond, BoundStatementNode *body,
                            BoundLabel *break_lbl, BoundLabel *cont_lbl);

  BoundExpressionNode *condition_expression() const;

  BoundStatementNode *body_statement() const;

  BoundLabel *break_label() const;

  BoundLabel *continue_label() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;
};

} // namespace Binding

#endif