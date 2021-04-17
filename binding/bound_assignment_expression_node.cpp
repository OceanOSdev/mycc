#include "bound_assignment_expression_node.h"
#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"

namespace Binding {

BoundAssignmentExpressionNode::BoundAssignmentExpressionNode(BoundAssignmentOpKind assignment_op_kind, Symbols::VariableSymbol* variable, BoundExpressionNode* expression) :
    m_assignment_op_kind(assignment_op_kind), m_variable(variable), m_expression(expression) {}


BoundAssignmentOpKind BoundAssignmentExpressionNode::assignment_op_kind() const {
    return m_assignment_op_kind;
}

Symbols::VariableSymbol* BoundAssignmentExpressionNode::variable() const {
    return m_variable;
}

BoundExpressionNode* BoundAssignmentExpressionNode::expression() const {
    return m_expression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundAssignmentExpressionNode::type() const {
    return m_expression->type();
} 

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundAssignmentExpressionNode::kind() const {
    return BoundNodeKind::AssignmentExpression;
}

}