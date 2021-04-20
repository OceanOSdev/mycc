#pragma once
#ifndef SYNTAX_IF_STATEMENT_NODE_H
#define SYNTAX_IF_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {
class SyntaxToken;
class IfStatementNode : public StatementNode {
private:
    ExpressionNode* if_expr;
    StatementNode* then_stmt;
    StatementNode* else_stmt; 
public:
    IfStatementNode(SyntaxToken* token, ExpressionNode* condition, StatementNode* t, StatementNode* e);
    
    ~IfStatementNode();

    /*
     * The conditional expression in this if statement.
     */
    ExpressionNode* condition() const;

    /*
     * The 'then' statement in this if statement.
     */
    StatementNode* then_statement() const;

    /*
     * The 'else' statement in this if statement.
     */
    StatementNode* else_statement() const;

    /*
     * Whether or not this if statement contains an 'else' block
     */
    bool has_else_statement() const;

    SyntaxKind kind() const override;
};

}

#endif