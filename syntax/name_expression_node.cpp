#include "name_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

NameExpressionNode::NameExpressionNode(SyntaxToken *token, std::string id)
    : ExpressionNode(token), identifier(id) {}

NameExpressionNode::~NameExpressionNode() {}

/*
 * The name of the identifier (as an l_val).
 */
std::string NameExpressionNode::name() const { return identifier; }

SyntaxKind NameExpressionNode::kind() const {
  return SyntaxKind::NameExpression;
}

} // namespace Syntax