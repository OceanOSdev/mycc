#include "expression_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

ExpressionStatementNode::ExpressionStatementNode(SyntaxToken *token,
                                                 ExpressionNode *e)
    : StatementNode(token), expr(e) {}

ExpressionStatementNode::~ExpressionStatementNode() { delete expr; }

/*
 * The expression contained in this statement.
 */
ExpressionNode *ExpressionStatementNode::expression() const { return expr; }

SyntaxKind ExpressionStatementNode::kind() const {
  return SyntaxKind::ExpressionStatement;
}

} // namespace Syntax