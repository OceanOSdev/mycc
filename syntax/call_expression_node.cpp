#include "call_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

CallExpressionNode::CallExpressionNode(SyntaxToken *token, std::string id,
                                       std::vector<ExpressionNode *> expr_list)
    : ExpressionNode(token), identifier(id), expression_list(expr_list) {}

CallExpressionNode::~CallExpressionNode() {
  std::vector<ExpressionNode *>::iterator expr_iter;
  for (expr_iter = expression_list.begin(); expr_iter != expression_list.end();
       expr_iter++)
    delete *expr_iter;
}

/*
 * The name of the method (lets be real thats whats getting called)
 * being called.
 */
std::string CallExpressionNode::name() const { return identifier; }

/*
 * The list of expressions being passed as arguments.
 */
std::vector<ExpressionNode *> CallExpressionNode::expressions() const {
  return expression_list;
}

SyntaxKind CallExpressionNode::kind() const {
  return SyntaxKind::CallExpression;
}

} // namespace Syntax