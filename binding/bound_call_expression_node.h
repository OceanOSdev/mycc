#pragma once
#ifndef BOUND_CALL_EXPRESSION_NODE_H
#define BOUND_CALL_EXPRESSION_NODE_H

#include "bound_expression_node.h"
#include <vector>

namespace Symbols {
    class FunctionSymbol;
}

namespace Binding {

class BoundCallExpressionNode : public BoundExpressionNode {
private:
    Symbols::FunctionSymbol* m_function;
    std::vector<BoundExpressionNode*> m_arguments;
public:
    BoundCallExpressionNode(Symbols::FunctionSymbol* function, std::vector<BoundExpressionNode*> arguments);

    Symbols::FunctionSymbol* function() const;

    std::vector<BoundExpressionNode*> arguments() const;

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