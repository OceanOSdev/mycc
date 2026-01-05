#include "global_struct_declaration_node.h"
#include "syntax_kind.h"

namespace Syntax {

GlobalStructDeclarationNode::GlobalStructDeclarationNode(
    SyntaxToken *token, StructDeclarationNode *struct_dec)
    : GlobalDeclarationNode(token), m_struct_declaration(struct_dec) {}

GlobalStructDeclarationNode::~GlobalStructDeclarationNode() {
  delete m_struct_declaration;
}

/*
 * The struct declaration.
 */
StructDeclarationNode *GlobalStructDeclarationNode::struct_declaration() const {
  return m_struct_declaration;
}

SyntaxKind GlobalStructDeclarationNode::kind() const {
  return SyntaxKind::GlobalStructDeclaration;
}

} // namespace Syntax