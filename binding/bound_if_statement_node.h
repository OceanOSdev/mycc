#pragma once
#ifndef BOUND_IF_STATEMENT_NODE_H
#define BOUND_IF_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {
class BoundExpressionNode;

class BoundIfStatementNode : public BoundStatementNode {
private:
    BoundExpressionNode* m_condition;
    BoundStatementNode* m_then_statement;
    BoundStatementNode* m_else_statement;
public:
    BoundIfStatementNode(BoundExpressionNode* condition, BoundStatementNode* then_stmt, BoundStatementNode* else_stmt);

    BoundExpressionNode* condition() const;

    BoundStatementNode* then_statement() const;

    BoundStatementNode* else_statement() const;

    bool has_else_statement() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

};


}

#endif