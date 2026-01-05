#include "unary_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

UnaryExpressionNode::UnaryExpressionNode(SyntaxToken *token, token_type_t tt,
                                         ExpressionNode *r_expr)
    : ExpressionNode(token), token_type(tt), expr(r_expr) {}

UnaryExpressionNode::~UnaryExpressionNode() { delete expr; }

/*
 * The binary operation stored as a token.
 */
token_type_t UnaryExpressionNode::syntax_token_type() const {
  return token_type;
}

/*
 * The expression.
 */
ExpressionNode *UnaryExpressionNode::expression() const { return expr; }

SyntaxKind UnaryExpressionNode::kind() const {
  return SyntaxKind::UnaryExpression;
}

} // namespace Syntax