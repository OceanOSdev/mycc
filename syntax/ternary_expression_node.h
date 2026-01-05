#pragma once
#ifndef SYNTAX_TERNARY_EXPRESSION_NODE_H
#define SYNTAX_TERNARY_EXPRESSION_NODE_H

#include "expression_node.h"

namespace Syntax {

class TernaryExpressionNode : public ExpressionNode {
private:
  ExpressionNode *mux_expr;
  ExpressionNode *true_expr;
  ExpressionNode *false_expr;

public:
  TernaryExpressionNode(SyntaxToken *token, ExpressionNode *expr,
                        ExpressionNode *t_expr, ExpressionNode *f_expr);

  ~TernaryExpressionNode();

  /*
   * The conditional expression.
   */
  ExpressionNode *conditional_expression() const;

  /*
   * The expression that the ternary expression evals to
   * if the conditional expression is true.
   */
  ExpressionNode *true_expression() const;

  /*
   * The expression that the ternary expression evals to
   * if the conditional expression is false.
   */
  ExpressionNode *false_expression() const;

  SyntaxKind kind() const override;
};

} // namespace Syntax

#endif