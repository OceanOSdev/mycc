#pragma once
#ifndef BOUND_INCREMENT_EXPRESSION_NODE_H
#define BOUND_INCREMENT_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Binding {

/*
 * ID => Increment/Decrement
 */
enum IDNotation : int { PREFIX, POSTFIX };

class BoundIncrementExpressionNode : public BoundExpressionNode {
private:
  IDNotation m_notation;
  BoundExpressionNode *m_expression;

public:
  BoundIncrementExpressionNode(IDNotation notation,
                               BoundExpressionNode *expression);

  IDNotation notation() const;

  BoundExpressionNode *expression() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;

  /*
   * The type that this expression evaluates to.
   */
  const Symbols::TypeSymbol *type() const override;
};

class BoundDecrementExpressionNode : public BoundExpressionNode {
private:
  IDNotation m_notation;
  BoundExpressionNode *m_expression;

public:
  BoundDecrementExpressionNode(IDNotation notation,
                               BoundExpressionNode *expression);

  IDNotation notation() const;

  BoundExpressionNode *expression() const;

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