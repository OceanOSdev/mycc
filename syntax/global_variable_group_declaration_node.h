#ifndef SYNTAX_GLOBAL_VARIABLE_GROUP_DECLARATION_NODE_H
#define SYNTAX_GLOBAL_VARIABLE_GROUP_DECLARATION_NODE_H

#include "global_declaration_node.h"
#include "variable_group_declaration_node.h"

namespace Syntax {

class GlobalVariableGroupDeclarationNode : public GlobalDeclarationNode {
private:
    VariableGroupDeclarationNode* m_variable_group;
public:
    GlobalVariableGroupDeclarationNode(VariableGroupDeclarationNode* var_group);
    
    ~GlobalVariableGroupDeclarationNode();

    /*
     * The variable group declarations.
     */
    VariableGroupDeclarationNode* variable_group() const;
};

}

#endif