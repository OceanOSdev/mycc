#pragma once
#ifndef BOUND_CONDITIONAL_GOTO_STATEMENT_NODE_H
#define BOUND_CONDITIONAL_GOTO_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {
class BoundLabel;
class BoundExpressionNode;

class BoundConditionalGotoStatementNode : public BoundStatementNode {
private:
  BoundLabel *m_label;
  BoundExpressionNode *m_condition;
  bool m_jump_if_true;

public:
  BoundConditionalGotoStatementNode(BoundLabel *label,
                                    BoundExpressionNode *condition,
                                    bool jump_if_true = true);

  BoundLabel *label() const;

  BoundExpressionNode *condition() const;

  bool jump_if_true() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;
};

} // namespace Binding

#endif