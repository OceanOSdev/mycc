#pragma once
#ifndef SYNTAX_UNARY_EXPRESSION_NODE_H
#define SYNTAX_UNARY_EXPRESSION_NODE_H

#include "expression_node.h"
#include "operator_tokens.h"

namespace Syntax {

class UnaryExpressionNode : public ExpressionNode {
private:
    SyntaxTokenType token_type;
    ExpressionNode* expr;
public:
    UnaryExpressionNode(SyntaxTokenType tt, ExpressionNode* r_expr);

    ~UnaryExpressionNode();

    /*
     * The binary operation stored as a token.
     */
    SyntaxTokenType syntax_token_type() const;

    /*
     * The expression.
     */
    ExpressionNode* expression() const;
};

}

#endif