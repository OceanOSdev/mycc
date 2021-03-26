#ifndef SYNTAX_WHILE_STATEMENT_NODE_H
#define SYNTAX_WHILE_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {

class WhileStatementNode : public StatementNode {
private:
    ExpressionNode* cond_expr;
    StatementNode* body_stmt;
public:
    WhileStatementNode(ExpressionNode* condition, StatementNode* body) : 
        cond_expr(condition), body_stmt(body) {}
    
    ~WhileStatementNode() { 
        delete cond_expr;
        delete body_stmt;
    }

    /*
     * The conditional expression that runs before every loop.
     */
    ExpressionNode* conditional_expression() const { return cond_expr; }

    /*
     * The body statement that runs every loop.
     */
    StatementNode* body_statement() const { return body_stmt; }
};

}

#endif