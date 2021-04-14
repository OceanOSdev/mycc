#include "break_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

BreakStatementNode::BreakStatementNode() { }
BreakStatementNode::~BreakStatementNode() { }

SyntaxKind BreakStatementNode::kind() const {
    return SyntaxKind::BreakStatement;
}

}