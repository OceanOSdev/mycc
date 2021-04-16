#pragma once
#ifndef BOUND_RETURN_STATEMENT_NODE_H
#define BOUND_RETURN_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {
class BoundExpressionNode;

class BoundReturnStatementNode : public BoundStatementNode {
private:
    BoundExpressionNode* m_expression;
public:
    BoundReturnStatementNode(BoundExpressionNode* expression);

    BoundExpressionNode* expression() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;
};

}

#endif