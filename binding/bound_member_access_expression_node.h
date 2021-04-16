#pragma once
#ifndef BOUND_MEMBER_ACCESS_EXPRESSION_NODE_H
#define BOUND_MEMBER_ACCESS_EXPRESSION_NODE_H

#include "bound_variable_reference_expression_node.h"

namespace Binding {

class BoundMemberAccessExpressionNode : public BoundVariableReferenceExpressionNode {
private:
    BoundVariableReferenceExpressionNode* m_encapsulating_variable_reference;
public:
    BoundMemberAccessExpressionNode(Symbols::VariableSymbol* variable_reference, BoundVariableReferenceExpressionNode* encapsulating_variable_reference);

    /*
     * A pointer to a variable of the type that encapsulates this member.
     */
    BoundVariableReferenceExpressionNode* encapsulating_variable_reference() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;
};

}

#endif