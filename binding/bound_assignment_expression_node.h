#pragma once
#ifndef BOUND_ASSIGNMENT_EXPRESSION_NODE_H
#define BOUND_ASSIGNMENT_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Symbols {
    class VariableSymbol;
}

namespace Binding {

enum BoundAssignmentOpKind : int {
    ASSIGN,
    PLUS_ASSIGN,
    MINUS_ASSIGN,
    STAR_ASSIGN,
    SLASH_ASSIGN
};

class BoundAssignmentExpressionNode : public BoundExpressionNode {
private:
    BoundAssignmentOpKind m_assignment_op_kind;
    Symbols::VariableSymbol* m_variable;
    BoundExpressionNode* m_expression;
public:
    BoundAssignmentExpressionNode(BoundAssignmentOpKind assignment_op_kind, Symbols::VariableSymbol* variable, BoundExpressionNode* expression);

    BoundAssignmentOpKind assignment_op_kind() const;

    Symbols::VariableSymbol* variable() const;

    BoundExpressionNode* expression() const;

    /*
     * The type that this expression evaluates to.
     */
    const Symbols::TypeSymbol* type() const override;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

};

}

#endif