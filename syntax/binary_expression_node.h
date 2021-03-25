#ifndef SYNTAX_BINARY_EXPRESSION_NODE_H
#define SYNTAX_BINARY_EXPRESSION_NODE_H

#include "expression_node.h"
#include "operator_tokens.h"

namespace Syntax {

class BinaryExpressionNode : public ExpressionNode {
private:
    ExpressionNode* left_expr;
    SyntaxTokenType token_type;
    ExpressionNode* right_expr;
public:
    BinaryExpressionNode(ExpressionNode* l_expr, SyntaxTokenType tt, ExpressionNode* r_expr) :
        left_expr(l_expr), token_type(tt), right_expr(r_expr) {}

    ~BinaryExpressionNode() {
        delete left_expr;
        delete right_expr;
    }

    /*
     * The expression on the left-hand side of the operator.
     */
    ExpressionNode* left_expression() const { return left_expr; }

    /*
     * The binary operation stored as a token.
     */
    SyntaxTokenType syntax_token_type() const { return token_type; }

    /*
     * The expression on the right-hand side of the operator.
     */
    ExpressionNode* right_expression() const { return right_expr; }
};

}

#endif