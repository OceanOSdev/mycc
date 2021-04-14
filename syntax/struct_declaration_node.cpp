#include "struct_declaration_node.h"
#include "../symbols/variable_symbol.h"
#include "syntax_kind.h"

namespace Syntax {

StructDeclarationNode::StructDeclarationNode(std::string id, std::vector<VariableGroupDeclarationNode*> vars) : 
    name(id), members(vars) { 

        for (auto mlist : vars) {
            auto v_sym_list = mlist->variable_list();
            m_members.insert(m_members.end(), v_sym_list.begin(), v_sym_list.end());
        }

}

StructDeclarationNode::~StructDeclarationNode() {
    std::vector<VariableGroupDeclarationNode*>::iterator member_iter;
    for (member_iter = members.begin(); member_iter != members.end(); member_iter++)
        delete *member_iter;
}

/*
 * The name of the struct.
 */
std::string StructDeclarationNode::struct_name() const { 
    return name; 
}

/*
 * The members (variables) that this struct contains.
 */
std::vector<VariableGroupDeclarationNode*> StructDeclarationNode::struct_members() const { 
    return members; 
}

/*
 * The members (variables) that this struct contains.
 * 
 * Note: This container uses the VariableSymbol type
 * to describe the members.
 */
std::vector<Symbols::VariableSymbol*> StructDeclarationNode::struct_member_list() const {
    return m_members;
}

SyntaxKind StructDeclarationNode::kind() const {
    return SyntaxKind::StructDeclaration;
}

}