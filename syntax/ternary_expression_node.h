#ifndef SYNTAX_TERNARY_EXPRESSION_NODE_H
#define SYNTAX_TERNARY_EXPRESSION_NODE_H

#include "expression_node.h"

namespace Syntax {

class TernaryExpressionNode : public ExpressionNode {
private:
    ExpressionNode* mux_expr;
    ExpressionNode* true_expr;
    ExpressionNode* false_expr;
public:
    TernaryExpressionNode(ExpressionNode* expr, ExpressionNode* t_expr, ExpressionNode* f_expr) :
        mux_expr(expr), true_expr(t_expr), false_expr(f_expr) {}

    ~TernaryExpressionNode() {
        delete mux_expr;
        delete true_expr;
        delete false_expr;
    }

    /*
     * The conditional expression.
     */
    ExpressionNode* conditional_expression() const { return mux_expr; }

    /*
     * The expression that the ternary expression evals to
     * if the conditional expression is true.
     */
    ExpressionNode* true_expression() const { return true_expr; }

    /*
     * The expression that the ternary expression evals to
     * if the conditional expression is false.
     */
    ExpressionNode* false_expression() const { return false_expr; }
};

}

#endif