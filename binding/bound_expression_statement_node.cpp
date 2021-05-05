#include "bound_expression_statement_node.h"
#include "bound_expression_node.h"
#include "bound_node_factory.h"

namespace Binding {

BoundExpressionStatementNode* Factory::statement(BoundExpressionNode* expression) {
    return new BoundExpressionStatementNode(expression);
}

BoundExpressionStatementNode::BoundExpressionStatementNode(BoundExpressionNode* expression) :
    m_expression(expression) {}

BoundExpressionStatementNode::~BoundExpressionStatementNode() {

}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundExpressionStatementNode::kind() const {
    return BoundNodeKind::ExpressionStatement;
}

BoundExpressionNode* BoundExpressionStatementNode::expression() const {
    return m_expression;
}

}