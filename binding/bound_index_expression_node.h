#pragma once
#ifndef BOUND_INDEX_EXPRESSION_NODE_H
#define BOUND_INDEX_EXPRESSION_NODE_H

#include "bound_variable_reference_expression_node.h"

namespace Binding {
class BoundIndexExpressionNode : public BoundVariableReferenceExpressionNode {
private:
    BoundExpressionNode* m_expression;
public:
    BoundIndexExpressionNode(Symbols::VariableSymbol* variable_reference, BoundExpressionNode* expression);

    /*
     * The expression that evaluates to the index of this index expression.
     */
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