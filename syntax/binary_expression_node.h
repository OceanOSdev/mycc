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
    BinaryExpressionNode(ExpressionNode* l_expr, SyntaxTokenType tt, ExpressionNode* r_expr);

    ~BinaryExpressionNode();

    /*
     * The expression on the left-hand side of the operator.
     */
    ExpressionNode* left_expression() const;

    /*
     * The binary operation stored as a token.
     */
    SyntaxTokenType syntax_token_type() const;

    /*
     * The expression on the right-hand side of the operator.
     */
    ExpressionNode* right_expression() const;
};

}

#endif