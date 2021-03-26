#ifndef SYNTAX_VARIABLE_GROUP_DECLARATION_NODE_H
#define SYNTAX_VARIABLE_GROUP_DECLARATION_NODE_H

#include <vector>
#include <string>
#include "local_declaration_node.h"
#include "expression_node.h"
#include "partial_variable_declaration_node.h"

namespace Syntax {

class VariableGroupDeclarationNode : public LocalDeclarationNode {
private:
    std::string group_type;
    std::vector<PartialVariableDeclarationNode*> dec_group;
    bool read_only;
    bool custom_type;
public:
    VariableGroupDeclarationNode(std::string type, std::vector<PartialVariableDeclarationNode*> vars, bool r_o, bool cust) :
        group_type(type), dec_group(vars), read_only(r_o), custom_type(cust) {}
    
    ~VariableGroupDeclarationNode() {
        std::vector<PartialVariableDeclarationNode*>::iterator dec_iter;
        for (dec_iter = dec_group.begin(); dec_iter != dec_group.end(); dec_iter++)
            delete *dec_iter;
    }

    /*
     * The type of all variables in this collection.
     */
    std::string type() const { return group_type; }

    /*
     * The list of all partial variable declarations in this group.
     */
    std::vector<PartialVariableDeclarationNode*> partial_variable_group () const { return dec_group; }

    /*
     * Whether or not all the variables are constant.
     */
    bool is_const() const { return read_only; }

    /*
     * Whether or not the variables are struct types.
     */
    bool is_struct() const { return custom_type; }
};

}

#endif