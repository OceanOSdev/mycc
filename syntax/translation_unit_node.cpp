#include "translation_unit_node.h"
#include "syntax_kind.h"

namespace Syntax {
TranslationUnitNode::TranslationUnitNode(
    SyntaxToken *token, std::vector<GlobalDeclarationNode *> glob_decs)
    : SyntaxNode(token), m_global_declarations(glob_decs) {}

TranslationUnitNode::~TranslationUnitNode() {
  std::vector<GlobalDeclarationNode *>::iterator gd_iter;
  for (gd_iter = m_global_declarations.begin();
       gd_iter != m_global_declarations.end(); gd_iter++)
    delete *gd_iter;
}

/*
 * List of global declarations in this translation unit.
 */
std::vector<GlobalDeclarationNode *>
TranslationUnitNode::global_declarations() const {
  return m_global_declarations;
}

SyntaxKind TranslationUnitNode::kind() const {
  return SyntaxKind::TranslationUnit;
}

} // namespace Syntax