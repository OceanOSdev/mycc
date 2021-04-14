#include "continue_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

ContinueStatementNode::ContinueStatementNode() {}
ContinueStatementNode::~ContinueStatementNode() {}

SyntaxKind ContinueStatementNode::kind() const {
    return SyntaxKind::ContinueStatement;
}

}