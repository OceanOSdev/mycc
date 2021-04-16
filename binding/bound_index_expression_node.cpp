#include "bound_index_expression_node.h"
#include "../symbols/type_symbol.h"

namespace Binding {

BoundIndexExpressionNode::BoundIndexExpressionNode(std::string identifier, BoundExpressionNode* expression) :
    m_identifier(identifier), m_expression(expression) {}
/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundIndexExpressionNode::type() const {
    return nullptr;
}
/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundIndexExpressionNode::kind() const {
    return BoundNodeKind::IndexExpression;
}

}