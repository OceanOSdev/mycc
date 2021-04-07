#pragma once
#ifndef BOUND_ERROR_EXPRESSION_NODE_H
#define BOUND_ERROR_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Binding {

class BoundErrorExpressionNode : public BoundExpressionNode {
public:
    BoundErrorExpressionNode();
    ~BoundErrorExpressionNode();

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

    /*
     * The type that this expression evaluates to.
     */
    Symbols::TypeSymbol type() const override;
};

}

#endif