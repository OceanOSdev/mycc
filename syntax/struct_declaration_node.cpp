#include "struct_declaration_node.h"

namespace Syntax {

StructDeclarationNode::StructDeclarationNode(std::string id, std::vector<VariableGroupDeclarationNode*> vars) : 
    name(id), members(vars) { }

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

}