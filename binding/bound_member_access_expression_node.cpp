#include "bound_member_access_expression_node.h"
#include "../symbols/variable_symbol.h"

namespace Binding {

BoundMemberAccessExpressionNode::BoundMemberAccessExpressionNode(Symbols::VariableSymbol* variable_reference, BoundVariableReferenceExpressionNode* encapsulating_variable_reference) :
    BoundVariableReferenceExpressionNode(variable_reference), m_encapsulating_variable_reference(encapsulating_variable_reference) {}

/*
 * A pointer to a variable of the type that encapsulates this member.
 */
BoundVariableReferenceExpressionNode* BoundMemberAccessExpressionNode::encapsulating_variable_reference() const {
    return m_encapsulating_variable_reference;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundMemberAccessExpressionNode::kind() const {
    return BoundNodeKind::MemberAccessExpression;
}

}