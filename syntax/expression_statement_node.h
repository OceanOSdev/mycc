#pragma once
#ifndef SYNTAX_EXPRESSION_STATEMENT_NODE_H
#define SYNTAX_EXPRESSION_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {
class SyntaxToken;
class ExpressionStatementNode : public StatementNode {
private:
    ExpressionNode* expr;
public:
    ExpressionStatementNode(SyntaxToken* token, ExpressionNode* e);
    ~ExpressionStatementNode();

    /*
     * The expression contained in this statement.
     */
    ExpressionNode* expression() const;

    SyntaxKind kind() const override;
};

}

#endif