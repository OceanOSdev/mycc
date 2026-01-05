#include "bound_error_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundErrorExpressionNode::BoundErrorExpressionNode() {}

BoundErrorExpressionNode::~BoundErrorExpressionNode() {}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundErrorExpressionNode::kind() const {
  return BoundNodeKind::ErrorExpression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol *BoundErrorExpressionNode::type() const {
  return &Symbols::TypeSymbol::Error;
}

} // namespace Binding