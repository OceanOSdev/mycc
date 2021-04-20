#include "bound_assignment_expression_node.h"
#include "bound_variable_reference_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundAssignmentExpressionNode::BoundAssignmentExpressionNode(BoundAssignmentOpKind assignment_op_kind, BoundVariableReferenceExpressionNode* variable, BoundExpressionNode* expression) :
    m_assignment_op_kind(assignment_op_kind), m_variable_reference(variable), m_expression(expression) {}


BoundAssignmentOpKind BoundAssignmentExpressionNode::assignment_op_kind() const {
    return m_assignment_op_kind;
}

BoundVariableReferenceExpressionNode* BoundAssignmentExpressionNode::variable_reference() const {
    return m_variable_reference;
}

BoundExpressionNode* BoundAssignmentExpressionNode::expression() const {
    return m_expression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundAssignmentExpressionNode::type() const {
    return m_variable_reference->type();
} 

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundAssignmentExpressionNode::kind() const {
    return BoundNodeKind::AssignmentExpression;
}

}