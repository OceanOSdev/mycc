#include "binary_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

BinaryExpressionNode::BinaryExpressionNode(SyntaxToken* token, ExpressionNode* l_expr, token_type_t tt, ExpressionNode* r_expr) :
    ExpressionNode(token), left_expr(l_expr), token_type(tt), right_expr(r_expr) {}

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
token_type_t BinaryExpressionNode::syntax_token_type() const { 
    return token_type; 
}

/*
 * The expression on the right-hand side of the operator.
 */
ExpressionNode* BinaryExpressionNode::right_expression() const { 
    return right_expr; 
}

SyntaxKind BinaryExpressionNode::kind() const {
    return SyntaxKind::BinaryExpression;
}

}