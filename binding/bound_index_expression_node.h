#pragma once
#ifndef BOUND_INDEX_EXPRESSION_NODE_H
#define BOUND_INDEX_EXPRESSION_NODE_H

#include "bound_expression_node.h"
#include <string>

namespace Binding {
class BoundIndexExpressionNode : public BoundExpressionNode {
private:
    std::string m_identifier;
    BoundExpressionNode* m_expression;
public:
    BoundIndexExpressionNode(std::string identifier, BoundExpressionNode* expression);

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