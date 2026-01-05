#include "bound_member_access_expression_node.h"
#include "../symbols/variable_symbol.h"

namespace Binding {

BoundMemberAccessExpressionNode::BoundMemberAccessExpressionNode(
    BoundVariableReferenceExpressionNode *encapsulated_variable_reference,
    BoundVariableReferenceExpressionNode *encapsulating_variable_reference)
    : BoundVariableReferenceExpressionNode(
          encapsulated_variable_reference->variable_reference()),
      m_encapsulated_variable_reference(encapsulated_variable_reference),
      m_encapsulating_variable_reference(encapsulating_variable_reference) {}

BoundVariableReferenceExpressionNode *
BoundMemberAccessExpressionNode::encapsulated_variable_reference() const {
  return m_encapsulated_variable_reference;
}

/*
 * A pointer to a variable of the type that encapsulates this member.
 */
BoundVariableReferenceExpressionNode *
BoundMemberAccessExpressionNode::encapsulating_variable_reference() const {
  return m_encapsulating_variable_reference;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol *BoundMemberAccessExpressionNode::type() const {
  return m_encapsulated_variable_reference->type();
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundMemberAccessExpressionNode::kind() const {
  return BoundNodeKind::MemberAccessExpression;
}

} // namespace Binding