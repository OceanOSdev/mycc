#include "bound_expression_statement_node.h"
#include "bound_expression_node.h"

namespace Binding {

BoundExpressionStatementNode::BoundExpressionStatementNode(BoundExpressionNode* expression) :
    m_expression(expression) {}

BoundExpressionStatementNode::~BoundExpressionStatementNode() {

}

BoundExpressionNode* BoundExpressionStatementNode::expression() const {
    return m_expression;
}

}