#pragma once
#ifndef SYNTAX_WHILE_STATEMENT_NODE_H
#define SYNTAX_WHILE_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {
class SyntaxToken;
class WhileStatementNode : public StatementNode {
private:
    ExpressionNode* cond_expr;
    StatementNode* body_stmt;
public:
    WhileStatementNode(SyntaxToken* token, ExpressionNode* condition, StatementNode* body);
    
    ~WhileStatementNode();

    /*
     * The conditional expression that runs before every loop.
     */
    ExpressionNode* conditional_expression() const;

    /*
     * The body statement that runs every loop.
     */
    StatementNode* body_statement() const;

    SyntaxKind kind() const override;
};

}

#endif