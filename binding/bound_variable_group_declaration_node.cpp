#include "bound_variable_group_declaration_node.h"
#include "../symbols/variable_symbol.h"
#include "bound_expression_node.h"

namespace Binding {

BoundVariableDeclarationNode::BoundVariableDeclarationNode(
    Symbols::VariableSymbol *variable_symbol, BoundExpressionNode *initializer)
    : m_variable_symbol(variable_symbol), m_initializer(initializer) {}

Symbols::VariableSymbol *BoundVariableDeclarationNode::variable_symbol() const {
  return m_variable_symbol;
}

/*
 * The bound expression that represent this variable's
 * initial value at declaration.
 */
BoundExpressionNode *BoundVariableDeclarationNode::initializer() const {
  return m_initializer;
}

bool BoundVariableDeclarationNode::is_initialized() const {
  return m_initializer != nullptr;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundVariableDeclarationNode::kind() const {
  return BoundNodeKind::VariableDeclaration;
}

BoundVariableGroupDeclarationNode::BoundVariableGroupDeclarationNode(
    std::vector<BoundVariableDeclarationNode *> variable_declarations)
    : m_variable_declarations(variable_declarations) {}

std::vector<BoundVariableDeclarationNode *>
BoundVariableGroupDeclarationNode::variable_declarations() const {
  return m_variable_declarations;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundVariableGroupDeclarationNode::kind() const {
  return BoundNodeKind::VariableGroupDeclaration;
}

} // namespace Binding