#ifndef SYNTAX_DO_WHILE_STATEMENT_NODE_H
#define SYNTAX_DO_WHILE_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {

class DoWhileStatementNode : public StatementNode {
private:
    StatementNode* body_stmt;
    ExpressionNode* cond_expr;
public:
    DoWhileStatementNode(StatementNode* body, ExpressionNode* condition) : 
        body_stmt(body), cond_expr(condition) {}
    
    ~DoWhileStatementNode() { 
        delete cond_expr;
        delete body_stmt;
    }
    
    /*
     * The body statement that runs every loop.
     */
    StatementNode* body_statement() const { return body_stmt; }

    /*
     * The conditional expression that runs before every loop.
     */
    ExpressionNode* conditional_expression() const { return cond_expr; }

};

}

#endif