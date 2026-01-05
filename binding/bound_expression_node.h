#pragma once
#ifndef BOUND_EXPRESSION_NODE_H
#define BOUND_EXPRESSION_NODE_H

#include "bound_node.h"

namespace Symbols {
class TypeSymbol;
}

namespace Binding {

class BoundExpressionNode : public BoundNode {
public:
  BoundExpressionNode() {}

  /*
   * The type that this expression evaluates to.
   */
  virtual const Symbols::TypeSymbol *type() const = 0;
};

} // namespace Binding

#endif