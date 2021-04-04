#include "ternary_expression_node.h"

namespace Syntax {

TernaryExpressionNode::TernaryExpressionNode(SyntaxToken* token, ExpressionNode* expr, ExpressionNode* t_expr, ExpressionNode* f_expr) : 
    ExpressionNode(token), mux_expr(expr), true_expr(t_expr), false_expr(f_expr) {}

TernaryExpressionNode::~TernaryExpressionNode() {
    delete mux_expr;
    delete true_expr;
    delete false_expr;
}

/*
 * The conditional expression.
 */
ExpressionNode* TernaryExpressionNode::conditional_expression() const { 
    return mux_expr; 
}

/*
 * The expression that the ternary expression evals to
 * if the conditional expression is true.
 */
ExpressionNode* TernaryExpressionNode::true_expression() const { 
    return true_expr; 
}

/*
 * The expression that the ternary expression evals to
 * if the conditional expression is false.
 */
ExpressionNode* TernaryExpressionNode::false_expression() const { 
    return false_expr; 
}


}