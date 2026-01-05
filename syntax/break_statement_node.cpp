#include "break_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

BreakStatementNode::BreakStatementNode(SyntaxToken *token)
    : StatementNode(token) {}

BreakStatementNode::~BreakStatementNode() {}

SyntaxKind BreakStatementNode::kind() const {
  return SyntaxKind::BreakStatement;
}

} // namespace Syntax