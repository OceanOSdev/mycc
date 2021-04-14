#pragma once
#ifndef SYNTAX_BINARY_EXPRESSION_NODE_H
#define SYNTAX_BINARY_EXPRESSION_NODE_H

#include "expression_node.h"
#include "syntax_token.h"

namespace Syntax {
class BinaryExpressionNode : public ExpressionNode {
private:
    ExpressionNode* left_expr;
    token_type_t token_type;
    ExpressionNode* right_expr;
public:
    BinaryExpressionNode(SyntaxToken* token, ExpressionNode* l_expr, token_type_t tt, ExpressionNode* r_expr);

    ~BinaryExpressionNode();

    /*
     * The expression on the left-hand side of the operator.
     */
    ExpressionNode* left_expression() const;

    /*
     * The binary operation stored as a token.
     */
    token_type_t syntax_token_type() const;

    /*
     * The expression on the right-hand side of the operator.
     */
    ExpressionNode* right_expression() const;

    SyntaxKind kind() const override;
};

}

#endif