#pragma once
#ifndef SYNTAX_FOR_STATEMENT_NODE_H
#define SYNTAX_FOR_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {
class SyntaxToken;
class ForStatementNode : public StatementNode {
private:
    ExpressionNode* init_expr;
    ExpressionNode* cond_expr;
    ExpressionNode* inc_expr;
    StatementNode* body_stmt;
public:
    ForStatementNode(SyntaxToken* token, ExpressionNode* ini, ExpressionNode* condition, ExpressionNode* inc, StatementNode* body);
    
    ~ForStatementNode();

    /*
     * The initial expression that runs at the start of the for loop.
     */
    ExpressionNode* initial_expression() const;

    /*
     * The conditional expression that runs before every loop.
     */
    ExpressionNode* conditional_expression() const;

    /*
     * The third expression that runs after every loop.
     */
    ExpressionNode* third_expression() const;

    /*
     * The body statement that runs every loop.
     */
    StatementNode* body_statement() const;

    /*
     * Whether or not this for loop contains an initial expression.
     */
    bool has_initial_expression() const;

    /*
     * Whether or not this for loop contains a conditional expression.
     */
    bool has_conditional_expression() const;

    /*
     * Whether or not this for loop contains a third expression.
     */
    bool has_third_expression() const;

    SyntaxKind kind() const override;
};

}

#endif