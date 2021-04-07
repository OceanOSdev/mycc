#include "bound_error_expression_node.h"

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
Symbols::TypeSymbol BoundErrorExpressionNode::type() const {
    return Symbols::TypeSymbol::Error;
}

}