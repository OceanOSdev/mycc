#include "bound_variable_reference_expression_node.h"
#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"
#include "bound_node_factory.h"

namespace Binding {

BoundVariableReferenceExpressionNode *
Factory::var_ref(Symbols::VariableSymbol *variable) {
  return new BoundVariableReferenceExpressionNode(variable);
}

BoundVariableReferenceExpressionNode::BoundVariableReferenceExpressionNode(
    Symbols::VariableSymbol *variable_reference)
    : m_variable_reference(variable_reference) {}

Symbols::VariableSymbol *
BoundVariableReferenceExpressionNode::variable_reference() const {
  return m_variable_reference;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol *BoundVariableReferenceExpressionNode::type() const {
  return m_variable_reference->var_type();
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundVariableReferenceExpressionNode::kind() const {
  return BoundNodeKind::VariableReferenceExpression;
}

} // namespace Binding