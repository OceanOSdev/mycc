#pragma once
#ifndef BOUND_FOR_STATEMENT_NODE_H
#define BOUND_FOR_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {
class BoundLabel;
class BoundExpressionNode;

class BoundForStatementNode : public BoundStatementNode {
private:
    BoundExpressionNode* m_init_expression;
    BoundExpressionNode* m_cond_expression;
    BoundExpressionNode* m_third_expression;
    BoundStatementNode* m_body_statement;
    BoundLabel* m_break_label;
    BoundLabel* m_continue_label;
public:
    BoundForStatementNode(BoundExpressionNode* init, 
                          BoundExpressionNode* cond, 
                          BoundExpressionNode* third,
                          BoundStatementNode* body,
                          BoundLabel* break_lbl,
                          BoundLabel* cont_lbl);

    BoundExpressionNode* initial_expression() const;

    bool has_initial_expression() const;

    BoundExpressionNode* condition_expression() const;

    bool has_condition_expression() const;

    BoundExpressionNode* third_expression() const;

    bool has_third_expression() const;

    BoundStatementNode* body_statement() const;

    BoundLabel* break_label() const;

    BoundLabel* continue_label() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

};

}

#endif