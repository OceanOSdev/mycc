#include "expression_node.h"
#include "../symbols/type_symbol.h"
namespace Syntax {

ExpressionNode::ExpressionNode(SyntaxToken *token)
    : SyntaxNode(token), m_expr_type(nullptr) {}

/*
 * This method allows us to bind type information on
 * expression nodes after/while the syntax tree is
 * being built.
 *
 * Note: Kinda hacky and I'm not super proud of it.
 */
void ExpressionNode::mutate_type(const Symbols::TypeSymbol *expr_type) {
  m_expr_type = expr_type;
}

/*
 * The "bound" type of this expression, initally nullptr
 */
const Symbols::TypeSymbol *ExpressionNode::expr_type() const {
  return m_expr_type;
}

} // namespace Syntax