#include "do_while_statement_node.h"

namespace Syntax {

DoWhileStatementNode::DoWhileStatementNode(StatementNode* body, ExpressionNode* condition) : 
    body_stmt(body), cond_expr(condition) {}

DoWhileStatementNode::~DoWhileStatementNode() {
    delete cond_expr;
    delete body_stmt;
}

/*
 * The body statement that runs every loop.
 */
StatementNode* DoWhileStatementNode::body_statement() const { 
    return body_stmt; 
}

/*
 * The conditional expression that runs before every loop.
 */
ExpressionNode* DoWhileStatementNode::conditional_expression() const { 
    return cond_expr; 
}


}