#include "assignment_expression_node.h"
#include "syntax_kind.h"
namespace Syntax {

AssignmentExpressionNode::AssignmentExpressionNode(SyntaxToken *token,
                                                   ExpressionNode *id_exp,
                                                   token_type_t assign,
                                                   ExpressionNode *expr)
    : ExpressionNode(token), lhs(id_exp), assignment_token(assign), rhs(expr) {}

AssignmentExpressionNode::~AssignmentExpressionNode() {
  delete lhs;
  delete rhs;
}

/*
 * The expression containing the identifier.
 */
ExpressionNode *AssignmentExpressionNode::identifier_expression() const {
  return lhs;
}

/*
 * The type of assignment expression being performed on
 * the identifier.
 */
token_type_t AssignmentExpressionNode::assignment_type() const {
  return assignment_token;
}

/*
 * The expression that the identifier will be assigned.
 */
ExpressionNode *AssignmentExpressionNode::expression() const { return rhs; }

SyntaxKind AssignmentExpressionNode::kind() const {
  return SyntaxKind::AssignmentExpression;
}

} // namespace Syntax