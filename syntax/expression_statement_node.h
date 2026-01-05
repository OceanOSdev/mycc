#pragma once
#ifndef SYNTAX_EXPRESSION_STATEMENT_NODE_H
#define SYNTAX_EXPRESSION_STATEMENT_NODE_H

#include "expression_node.h"
#include "statement_node.h"

namespace Syntax {
class SyntaxToken;
class ExpressionStatementNode : public StatementNode {
private:
  ExpressionNode *expr;

public:
  ExpressionStatementNode(SyntaxToken *token, ExpressionNode *e);
  ~ExpressionStatementNode();

  /*
   * The expression contained in this statement.
   */
  ExpressionNode *expression() const;

  SyntaxKind kind() const override;
};

} // namespace Syntax

#endif