#include "cast_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

CastExpressionNode::CastExpressionNode(SyntaxToken *token, std::string type,
                                       ExpressionNode *expr)
    : ExpressionNode(token), casting_type(type), expression_node(expr) {}

CastExpressionNode::~CastExpressionNode() { delete expression_node; }

/*
 * The type that the expression r-value gets cast to.
 */
std::string CastExpressionNode::type() const { return casting_type; }

/*
 * The expression being casted.
 */
ExpressionNode *CastExpressionNode::expression() const {
  return expression_node;
}

SyntaxKind CastExpressionNode::kind() const {
  return SyntaxKind::CastExpression;
}

} // namespace Syntax