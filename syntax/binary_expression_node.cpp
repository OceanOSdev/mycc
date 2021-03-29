#include "binary_expression_node.h"

namespace Syntax {

BinaryExpressionNode::BinaryExpressionNode(ExpressionNode* l_expr, SyntaxTokenType tt, ExpressionNode* r_expr) :
    left_expr(l_expr), token_type(tt), right_expr(r_expr) {}

BinaryExpressionNode::~BinaryExpressionNode() {
    delete left_expr;
    delete right_expr;
}

/*
 * The expression on the left-hand side of the operator.
 */
ExpressionNode* BinaryExpressionNode::left_expression() const { 
    return left_expr; 
}

/*
 * The binary operation stored as a token.
 */
SyntaxTokenType BinaryExpressionNode::syntax_token_type() const { 
    return token_type; 
}

/*
 * The expression on the right-hand side of the operator.
 */
ExpressionNode* BinaryExpressionNode::right_expression() const { 
    return right_expr; 
}



}