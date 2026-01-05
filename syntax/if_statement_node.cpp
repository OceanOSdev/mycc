#include "if_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

IfStatementNode::IfStatementNode(SyntaxToken *token, ExpressionNode *condition,
                                 StatementNode *t, StatementNode *e)
    : StatementNode(token), if_expr(condition), then_stmt(t), else_stmt(e) {}

IfStatementNode::~IfStatementNode() {
  delete if_expr;
  delete then_stmt;
  if (has_else_statement())
    delete else_stmt;
}

/*
 * The conditional expression in this if statement.
 */
ExpressionNode *IfStatementNode::condition() const { return if_expr; }

/*
 * The 'then' statement in this if statement.
 */
StatementNode *IfStatementNode::then_statement() const { return then_stmt; }

/*
 * The 'else' statement in this if statement.
 */
StatementNode *IfStatementNode::else_statement() const { return else_stmt; }

/*
 * Whether or not this if statement contains an 'else' block
 */
bool IfStatementNode::has_else_statement() const {
  return else_stmt != nullptr;
}

SyntaxKind IfStatementNode::kind() const { return SyntaxKind::IfStatement; }

} // namespace Syntax