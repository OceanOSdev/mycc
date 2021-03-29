#include "variable_group_declaration_node.h"
namespace Syntax{

VariableGroupDeclarationNode::VariableGroupDeclarationNode(std::string type, std::vector<PartialVariableDeclarationNode*> vars, bool r_o, bool cust) :
   group_type(type), dec_group(vars), read_only(r_o), custom_type(cust) {}


VariableGroupDeclarationNode::~VariableGroupDeclarationNode() {
   std::vector<PartialVariableDeclarationNode*>::iterator dec_iter;
   for (dec_iter = dec_group.begin(); dec_iter != dec_group.end(); dec_iter++)
       delete *dec_iter;
}

/*
 * The type of all variables in this collection.
 */
std::string VariableGroupDeclarationNode::type() const { 
    return group_type; 
}

/*
 * The list of all partial variable declarations in this group.
 */
std::vector<PartialVariableDeclarationNode*> VariableGroupDeclarationNode::partial_variable_group () const { 
    return dec_group; 
}

/*
 * Whether or not all the variables are constant.
 */
bool VariableGroupDeclarationNode::is_const() const { 
    return read_only; 
}

/*
 * Whether or not the variables are struct types.
 */
bool VariableGroupDeclarationNode::is_struct() const { 
    return custom_type; 
}

}