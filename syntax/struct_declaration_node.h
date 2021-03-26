#ifndef SYNTAX_STRUCT_DECLARATION_NODE_H
#define SYNTAX_STRUCT_DECLARATION_NODE_H

#include <string>
#include <vector>
#include "local_declaration_node.h"
#include "variable_group_declaration_node.h"

namespace Syntax {

class StructDeclarationNode : public LocalDeclarationNode {
private:
    std::string name;
    std::vector<VariableGroupDeclarationNode*> members;
public:
    StructDeclarationNode(std::string id, std::vector<VariableGroupDeclarationNode*> vars) :
        name(id), members(vars) { }

    ~StructDeclarationNode() {
        std::vector<VariableGroupDeclarationNode*>::iterator member_iter;
        for (member_iter = members.begin(); member_iter != members.end(); member_iter++)
            delete *member_iter;
    }

    /*
     * The name of the struct.
     */
    std::string struct_name() const { return name; }

    /*
     * The members (variables) that this struct contains.
     */
    std::vector<VariableGroupDeclarationNode*> struct_members() const { return members; }

};

}

#endif