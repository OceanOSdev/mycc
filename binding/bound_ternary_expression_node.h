#pragma once
#ifndef BOUND_TERNARY_EXPRESSION_NODE_H
#define BOUND_TERNARY_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Binding {

class BoundTernaryExpressionNode : public BoundExpressionNode {
private:
  const Symbols::TypeSymbol *m_type;
  BoundExpressionNode *m_conditional;
  BoundExpressionNode *m_true_expr;
  BoundExpressionNode *m_false_expr;

public:
  BoundTernaryExpressionNode(const Symbols::TypeSymbol *type,
                             BoundExpressionNode *conditional,
                             BoundExpressionNode *true_expr,
                             BoundExpressionNode *false_expr);

  BoundExpressionNode *conditional() const;

  BoundExpressionNode *true_expr() const;

  BoundExpressionNode *false_expr() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;

  /*
   * The type that this expression evaluates to.
   */
  const Symbols::TypeSymbol *type() const override;
};

} // namespace Binding

#endif