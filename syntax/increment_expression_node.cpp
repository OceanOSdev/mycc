#include "increment_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

IncrementExpressionNode::IncrementExpressionNode(SyntaxToken *token,
                                                 ExpressionNode *id, bool post)
    : ExpressionNode(token), id_expr(id), is_post_increment(post) {}

IncrementExpressionNode::~IncrementExpressionNode() { delete id_expr; }

/*
 * The expression containing the identifier.
 */
ExpressionNode *IncrementExpressionNode::identifier_expression() const {
  return id_expr;
}

/*
 * Indicates whether we are post or pre incrementing.
 *
 * Post-increment: true.
 *
 * Pre-increment: false.
 */
bool IncrementExpressionNode::is_post() const { return is_post_increment; }

SyntaxKind IncrementExpressionNode::kind() const {
  return SyntaxKind::IncrementExpression;
}

} // namespace Syntax