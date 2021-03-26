#ifndef SYNTAX_IF_STATEMENT_NODE_H
#define SYNTAX_IF_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {

class IfStatementNode : public StatementNode {
private:
    ExpressionNode* if_expr;
    StatementNode* then_stmt;
    StatementNode* else_stmt; 
public:
    IfStatementNode(ExpressionNode* condition, StatementNode* t, StatementNode* e) : 
        if_expr(condition), then_stmt(t), else_stmt(e) {}
    
    ~IfStatementNode() { 
        delete if_expr;
        delete then_stmt;
        if (has_else_statement())
            delete else_stmt; 
    }

    /*
     * The conditional expression in this if statement.
     */
    ExpressionNode* condition() const { return if_expr; }

    /*
     * The 'then' statement in this if statement.
     */
    StatementNode* then_statement() const { return then_stmt; }

    /*
     * The 'else' statement in this if statement.
     */
    StatementNode* else_statement() const { return else_stmt; }

    /*
     * Whether or not this if statement contains an 'else' block
     */
    bool has_else_statement() const { return else_stmt != nullptr; }
};

}

#endif