#pragma once
#ifndef BOUND_ASSIGNMENT_EXPRESSION_NODE_H
#define BOUND_ASSIGNMENT_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Binding {

enum BoundAssignmentOpKind : int {
  ASSIGN,
  PLUS_ASSIGN,
  MINUS_ASSIGN,
  STAR_ASSIGN,
  SLASH_ASSIGN
};

class BoundVariableReferenceExpressionNode;

class BoundAssignmentExpressionNode : public BoundExpressionNode {
private:
  BoundAssignmentOpKind m_assignment_op_kind;
  BoundVariableReferenceExpressionNode *m_variable_reference;
  BoundExpressionNode *m_expression;

public:
  BoundAssignmentExpressionNode(BoundAssignmentOpKind assignment_op_kind,
                                BoundVariableReferenceExpressionNode *variable,
                                BoundExpressionNode *expression);

  BoundAssignmentOpKind assignment_op_kind() const;

  BoundVariableReferenceExpressionNode *variable_reference() const;

  BoundExpressionNode *expression() const;

  /*
   * The type that this expression evaluates to.
   */
  const Symbols::TypeSymbol *type() const override;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;
};

} // namespace Binding

#endif