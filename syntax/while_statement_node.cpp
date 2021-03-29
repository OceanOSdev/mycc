#include "while_statement_node.h"

namespace Syntax {

WhileStatementNode::WhileStatementNode(ExpressionNode* condition, StatementNode* body) : 
        cond_expr(condition), body_stmt(body) {}

WhileStatementNode::~WhileStatementNode() { 
    delete cond_expr;
    delete body_stmt;
}

/*
 * The conditional expression that runs before every loop.
 */
ExpressionNode* WhileStatementNode::conditional_expression() const { 
    return cond_expr; 
}

/*
 * The body statement that runs every loop.
 */
StatementNode* WhileStatementNode::body_statement() const { 
    return body_stmt; 
}

}