#include "global_variable_group_declaration_node.h"

namespace Syntax {

GlobalVariableGroupDeclarationNode::GlobalVariableGroupDeclarationNode(VariableGroupDeclarationNode* var_group) :
    m_variable_group(var_group) { }

GlobalVariableGroupDeclarationNode::~GlobalVariableGroupDeclarationNode() {
    delete m_variable_group;
}

/*
 * The variable group declarations.
 */
VariableGroupDeclarationNode* GlobalVariableGroupDeclarationNode::variable_group() const { return m_variable_group; }

}