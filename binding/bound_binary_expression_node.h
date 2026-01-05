#pragma once
#ifndef BOUND_BINARY_EXPRESSION_NODE_H
#define BOUND_BINARY_EXPRESSION_NODE_H

#include "bound_binary_op_kind.h"
#include "bound_expression_node.h"

namespace Syntax {
enum btokentype : int;
}

namespace Binding {

class BoundBinaryOperatorNode {
private:
  BoundBinaryOpKind m_op_kind;
  const Symbols::TypeSymbol *m_left_type;
  const Symbols::TypeSymbol *m_right_type;
  const Symbols::TypeSymbol *m_type;
  static BoundBinaryOperatorNode *
  BindITypeOperator(BoundBinaryOpKind opKind, const Symbols::TypeSymbol *left,
                    const Symbols::TypeSymbol *right);
  static BoundBinaryOperatorNode *
  BindNTypeOperator(BoundBinaryOpKind opKind, const Symbols::TypeSymbol *left,
                    const Symbols::TypeSymbol *right);
  static BoundBinaryOperatorNode *
  BindNTypeCompOperator(BoundBinaryOpKind opKind,
                        const Symbols::TypeSymbol *left,
                        const Symbols::TypeSymbol *right);

public:
  BoundBinaryOperatorNode(BoundBinaryOpKind op, const Symbols::TypeSymbol *left,
                          const Symbols::TypeSymbol *right,
                          const Symbols::TypeSymbol *type);
  static BoundBinaryOperatorNode *Bind(Syntax::btokentype syntax_token_type,
                                       const Symbols::TypeSymbol *left,
                                       const Symbols::TypeSymbol *right);

  /*
   * The type symbol for the left side of the expression.
   */
  const Symbols::TypeSymbol *left_type() const;

  /*
   * The operator kind.
   */
  BoundBinaryOpKind op_kind() const;

  /*
   * The type symbol for the right side of the expression.
   */
  const Symbols::TypeSymbol *right_type() const;

  /*
   * The type that the binary expression operator returns.
   */
  const Symbols::TypeSymbol *type() const;
};

class BoundBinaryExpressionNode : public BoundExpressionNode {
private:
  BoundBinaryOperatorNode *m_op;
  BoundExpressionNode *m_left;
  BoundExpressionNode *m_right;

public:
  BoundBinaryExpressionNode(BoundBinaryOperatorNode *op,
                            BoundExpressionNode *left,
                            BoundExpressionNode *right);
  ~BoundBinaryExpressionNode();

  /*
   * The bound operator.
   */
  BoundBinaryOperatorNode *op() const;

  /*
   * The bound expression on the left-hand side of the operator.
   */
  BoundExpressionNode *left() const;

  /*
   * The bound expression on the right-hand side of the operator.
   */
  BoundExpressionNode *right() const;

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