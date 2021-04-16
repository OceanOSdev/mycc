#pragma once
#ifndef BOUND_VARIABLE_REFERENCE_EXPRESSION_NODE_H
#define BOUND_VARIABLE_REFERENCE_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Symbols {
    class VariableSymbol;
}

namespace Binding {

class BoundVariableReferenceExpressionNode : public BoundExpressionNode {
protected:
    Symbols::VariableSymbol* m_variable_reference;
public:
    BoundVariableReferenceExpressionNode(Symbols::VariableSymbol* variable_reference);

    Symbols::VariableSymbol* variable_reference() const;

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