#include "global_struct_declaration_node.h"

namespace Syntax {

GlobalStructDeclarationNode::GlobalStructDeclarationNode(StructDeclarationNode* struct_dec) :
    m_struct_declaration(struct_dec) {}

GlobalStructDeclarationNode::~GlobalStructDeclarationNode() {
    delete m_struct_declaration;
}

/*
 * The struct declaration.
 */
StructDeclarationNode* GlobalStructDeclarationNode::struct_declaration() const { 
    return m_struct_declaration; 
}

}