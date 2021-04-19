#pragma once
#ifndef BOUND_MEMBER_ACCESS_EXPRESSION_NODE_H
#define BOUND_MEMBER_ACCESS_EXPRESSION_NODE_H

#include "bound_variable_reference_expression_node.h"

namespace Binding {

class BoundMemberAccessExpressionNode : public BoundVariableReferenceExpressionNode {
private:
    BoundVariableReferenceExpressionNode* m_encapsulated_variable_reference;
    BoundVariableReferenceExpressionNode* m_encapsulating_variable_reference;
public:
    BoundMemberAccessExpressionNode(BoundVariableReferenceExpressionNode* encapsulated_variable_reference, BoundVariableReferenceExpressionNode* encapsulating_variable_reference);

    BoundVariableReferenceExpressionNode* encapsulated_variable_reference() const;

    /*
     * A pointer to a variable of the type that encapsulates this member.
     */
    BoundVariableReferenceExpressionNode* encapsulating_variable_reference() const;

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