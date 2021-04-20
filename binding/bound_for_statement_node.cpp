#include "bound_for_statement_node.h"
#include "bound_expression_node.h"
#include "bound_label_statement_node.h"

namespace Binding {

BoundForStatementNode::BoundForStatementNode(BoundExpressionNode* init, 
                                             BoundExpressionNode* cond, 
                                             BoundExpressionNode* third,
                                             BoundStatementNode* body,
                                             BoundLabel* break_lbl,
                                             BoundLabel* cont_lbl) :
    m_init_expression(init),
    m_cond_expression(cond),
    m_third_expression(third),
    m_body_statement(body),
    m_break_label(break_lbl),
    m_continue_label(cont_lbl) {}

BoundExpressionNode* BoundForStatementNode::initial_expression() const {
    return m_init_expression;
}

bool BoundForStatementNode::has_initial_expression() const {
    return m_init_expression != nullptr;
}

BoundExpressionNode* BoundForStatementNode::condition_expression() const {
    return m_cond_expression;
}

bool BoundForStatementNode::has_condition_expression() const {
    return m_cond_expression != nullptr;
}

BoundExpressionNode* BoundForStatementNode::third_expression() const {
    return m_third_expression;
}

bool BoundForStatementNode::has_third_expression() const {
    return m_third_expression != nullptr;
}

BoundStatementNode* BoundForStatementNode::body_statement() const {
    return m_body_statement;
}

BoundLabel* BoundForStatementNode::break_label() const {
    return m_break_label;
}

BoundLabel* BoundForStatementNode::continue_label() const {
    return m_continue_label;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundForStatementNode::kind() const {
    return BoundNodeKind::ForStatement;
}

}