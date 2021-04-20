#include "bound_conditional_goto_statement_node.h"
#include "bound_label_statement_node.h"
#include "bound_expression_node.h"

namespace Binding {

BoundConditionalGotoStatementNode::BoundConditionalGotoStatementNode(BoundLabel* label, BoundExpressionNode* condition, bool jump_if_true) :
    m_label(label),
    m_condition(condition),
    m_jump_if_true(jump_if_true) {}

BoundLabel* BoundConditionalGotoStatementNode::label() const {
    return m_label;
}

BoundExpressionNode* BoundConditionalGotoStatementNode::condition() const {
    return m_condition;
}

bool BoundConditionalGotoStatementNode::jump_if_true() const { 
    return m_jump_if_true;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundConditionalGotoStatementNode::kind() const {
    return BoundNodeKind::ConditionalGotoStatement;
}

}