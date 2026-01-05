#include "continue_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

ContinueStatementNode::ContinueStatementNode(SyntaxToken *token)
    : StatementNode(token) {}
ContinueStatementNode::~ContinueStatementNode() {}

SyntaxKind ContinueStatementNode::kind() const {
  return SyntaxKind::ContinueStatement;
}

} // namespace Syntax