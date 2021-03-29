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
    StructDeclarationNode(std::string id, std::vector<VariableGroupDeclarationNode*> vars);

    ~StructDeclarationNode();

    /*
     * The name of the struct.
     */
    std::string struct_name() const;

    /*
     * The members (variables) that this struct contains.
     */
    std::vector<VariableGroupDeclarationNode*> struct_members() const;

};

}

#endif