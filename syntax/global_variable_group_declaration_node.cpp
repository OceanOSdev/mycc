#include "global_variable_group_declaration_node.h"
#include "syntax_kind.h"

namespace Syntax {

GlobalVariableGroupDeclarationNode::GlobalVariableGroupDeclarationNode(SyntaxToken* token, VariableGroupDeclarationNode* var_group) :
    GlobalDeclarationNode(token), m_variable_group(var_group) { }

GlobalVariableGroupDeclarationNode::~GlobalVariableGroupDeclarationNode() {
    delete m_variable_group;
}

/*
 * The variable group declarations.
 */
VariableGroupDeclarationNode* GlobalVariableGroupDeclarationNode::variable_group() const { return m_variable_group; }

SyntaxKind GlobalVariableGroupDeclarationNode::kind() const {
    return SyntaxKind::GlobalVariableDeclaration;
}

}