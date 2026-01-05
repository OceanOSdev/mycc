#include "index_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

IndexExpressionNode::IndexExpressionNode(SyntaxToken *token, std::string id,
                                         ExpressionNode *e)
    : ExpressionNode(token), identifier(id), expr(e) {}

IndexExpressionNode::~IndexExpressionNode() { delete expr; }

/*
 * The name of the identifier (as an l_val).
 */
std::string IndexExpressionNode::name() const { return identifier; }

/*
 * The expression that evaluates to the index.
 */
ExpressionNode *IndexExpressionNode::expression() const { return expr; }

SyntaxKind IndexExpressionNode::kind() const {
  return SyntaxKind::IndexExpression;
}

} // namespace Syntax