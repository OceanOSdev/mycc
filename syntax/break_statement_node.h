#pragma once
#ifndef SYNTAX_BREAK_STATEMENT_NODE_H
#define SYNTAX_BREAK_STATEMENT_NODE_H

#include "statement_node.h"

namespace Syntax {
class SyntaxToken;
class BreakStatementNode : public StatementNode {
public:
  BreakStatementNode(SyntaxToken *token);
  ~BreakStatementNode();
  SyntaxKind kind() const override;
};

} // namespace Syntax

#endif