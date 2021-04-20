#pragma once
#ifndef SYNTAX_UNARY_EXPRESSION_NODE_H
#define SYNTAX_UNARY_EXPRESSION_NODE_H

#include "expression_node.h"
#include "syntax_token.h"

namespace Syntax {

class UnaryExpressionNode : public ExpressionNode {
private:
    token_type_t token_type;
    ExpressionNode* expr;
public:
    UnaryExpressionNode(SyntaxToken* token, token_type_t tt, ExpressionNode* r_expr);

    ~UnaryExpressionNode();

    /*
     * The binary operation stored as a token.
     */
    token_type_t syntax_token_type() const;

    /*
     * The expression.
     */
    ExpressionNode* expression() const;

    SyntaxKind kind() const override;
};

}

#endif