#include "bound_ternary_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundTernaryExpressionNode::BoundTernaryExpressionNode(const Symbols::TypeSymbol* type,
                                                       BoundExpressionNode* conditional,
                                                       BoundExpressionNode* true_expr,
                                                       BoundExpressionNode* false_expr) :
    m_type(type), m_conditional(conditional), m_true_expr(true_expr), m_false_expr(false_expr) {}

BoundExpressionNode* BoundTernaryExpressionNode::conditional() const {
    return m_conditional;
}

BoundExpressionNode* BoundTernaryExpressionNode::true_expr() const {
    return m_true_expr;
}

BoundExpressionNode* BoundTernaryExpressionNode::false_expr() const {
    return m_false_expr;
}
    
/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundTernaryExpressionNode::kind() const {
    return BoundNodeKind::TernaryExpression;
}
/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundTernaryExpressionNode::type() const {
    return m_type;
}

}