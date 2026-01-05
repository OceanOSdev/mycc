#include "literal_val_expression_node.h"
#include "syntax_kind.h"
#include "syntax_token.h"

namespace Syntax {

LiteralValExpressionNode::LiteralValExpressionNode(SyntaxToken *token)
    : ExpressionNode(token) {}

LiteralValExpressionNode::~LiteralValExpressionNode() {}

/*
 * An enum value representing the type stored in this node.
 */
token_data_type LiteralValExpressionNode::value_type() const {
  return m_token->get_type();
}

/*
 * The int value (if the type stored is an int).
 */
int LiteralValExpressionNode::int_value() const {
  return m_token->get_int_value();
}

/*
 * The float value (if the type stored is a float).
 */
float LiteralValExpressionNode::float_value() const {
  return m_token->get_float_value();
}

/*
 * The string value (if the type stored is a string).
 */
std::string LiteralValExpressionNode::string_value() const {
  return m_token->get_str_value();
}

/*
 * The char value (if the type stored is a char).
 */
char LiteralValExpressionNode::char_value() const {
  return m_token->get_char_value();
}

SyntaxKind LiteralValExpressionNode::kind() const {
  return SyntaxKind::LiteralValExpression;
}

} // namespace Syntax