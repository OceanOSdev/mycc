#pragma once
#ifndef SYNTAX_ASSIGNMENT_EXPRESSION_NODE_H
#define SYNTAX_ASSIGNMENT_EXPRESSION_NODE_H

#include "expression_node.h"
#include "syntax_token.h"

namespace Syntax {

class AssignmentExpressionNode : public ExpressionNode {
private:
  ExpressionNode *lhs;
  token_type_t assignment_token;
  ExpressionNode *rhs;

public:
  AssignmentExpressionNode(SyntaxToken *token, ExpressionNode *id_exp,
                           token_type_t assign, ExpressionNode *expr);

  ~AssignmentExpressionNode();

  /*
   * The expression containing the identifier.
   */
  ExpressionNode *identifier_expression() const;

  /*
   * The type of assignment expression being performed on
   * the identifier.
   */
  token_type_t assignment_type() const;

  /*
   * The expression that the identifier will be assigned.
   */
  ExpressionNode *expression() const;

  SyntaxKind kind() const override;
};

} // namespace Syntax
#endif