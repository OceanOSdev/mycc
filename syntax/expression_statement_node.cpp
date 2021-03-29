#include "expression_statement_node.h"

namespace Syntax {

ExpressionStatementNode::ExpressionStatementNode(ExpressionNode* e) : 
    expr(e) {}

ExpressionStatementNode::~ExpressionStatementNode() {
    delete expr; 
}

/*
 * The expression contained in this statement.
 */
ExpressionNode* ExpressionStatementNode::expression() const { 
    return expr; 
}

}