#pragma once
#ifndef BOUND_EXPRESSION_NODE_H
#define BOUND_EXPRESSION_NODE_H

#include "bound_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

class BoundExpressionNode : public BoundNode {
public:
    BoundExpressionNode() {}

    /*
     * The type that this expression evaluates to.
     */
    virtual Symbols::TypeSymbol type() const = 0;
};

}

#endif