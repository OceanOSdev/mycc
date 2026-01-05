#pragma once
#ifndef BOUND_EXPRESSION_STATEMENT_NODE_H
#define BOUND_EXPRESSION_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {
class BoundExpressionNode;

class BoundExpressionStatementNode : public BoundStatementNode {
private:
  BoundExpressionNode *m_expression;

public:
  BoundExpressionStatementNode(BoundExpressionNode *expression);
  ~BoundExpressionStatementNode();

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;

  BoundExpressionNode *expression() const;
};

} // namespace Binding

#endif