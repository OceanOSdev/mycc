#pragma once
#ifndef BOUND_CAST_EXPRESSION_NODE_H
#define BOUND_CAST_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Binding {

class BoundCastExpressionNode : public BoundExpressionNode {
private:
    const Symbols::TypeSymbol* m_cast_type;
    BoundExpressionNode* m_expression;
public:
    BoundCastExpressionNode(const Symbols::TypeSymbol* cast_type, BoundExpressionNode* expression);

    BoundExpressionNode* expression() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

    /*
     * The type that this expression evaluates to.
     */
    const Symbols::TypeSymbol* type() const override;
};

}

#endif