#pragma once
#ifndef SYNTAX_DO_WHILE_STATEMENT_NODE_H
#define SYNTAX_DO_WHILE_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {
class SyntaxToken;
class DoWhileStatementNode : public StatementNode {
private:
    StatementNode* body_stmt;
    ExpressionNode* cond_expr;
public:
    DoWhileStatementNode(SyntaxToken* token, StatementNode* body, ExpressionNode* condition);
    
    ~DoWhileStatementNode();
    
    /*
     * The body statement that runs every loop.
     */
    StatementNode* body_statement() const;

    /*
     * The conditional expression that runs before every loop.
     */
    ExpressionNode* conditional_expression() const;

    SyntaxKind kind() const override;

};

}

#endif