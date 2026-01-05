#include "bound_struct_declaration_node.h"
#include "../symbols/struct_symbol.h"

namespace Binding {

BoundStructDeclarationNode::BoundStructDeclarationNode(
    Symbols::StructSymbol *struct_symbol)
    : m_struct_symbol(struct_symbol) {}

Symbols::StructSymbol *BoundStructDeclarationNode::struct_symbol() const {
  return m_struct_symbol;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundStructDeclarationNode::kind() const {
  return BoundNodeKind::StructDeclaration;
}

} // namespace Binding