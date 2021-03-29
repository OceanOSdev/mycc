#include "unary_expression_node.h"

namespace Syntax {

UnaryExpressionNode::UnaryExpressionNode(SyntaxTokenType tt, ExpressionNode* r_expr) :
        token_type(tt), expr(r_expr) {}

UnaryExpressionNode::~UnaryExpressionNode() {
    delete expr;
}

/*
 * The binary operation stored as a token.
 */
SyntaxTokenType UnaryExpressionNode::syntax_token_type() const { 
    return token_type; 
}

/*
 * The expression.
 */
ExpressionNode* UnaryExpressionNode::expression() const { 
    return expr; 
}
}