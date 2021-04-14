#pragma once
#ifndef SYNTAX_VARIABLE_GROUP_DECLARATION_NODE_H
#define SYNTAX_VARIABLE_GROUP_DECLARATION_NODE_H

#include <vector>
#include <string>
#include "local_declaration_node.h"
#include "expression_node.h"
#include "partial_variable_declaration_node.h"

namespace Symbols {
class VariableSymbol;
}

namespace Syntax {

class VariableGroupDeclarationNode : public LocalDeclarationNode {
private:
    std::string group_type;
    std::vector<PartialVariableDeclarationNode*> dec_group;
    bool read_only;
    bool custom_type;
    std::vector<Symbols::VariableSymbol*> m_vars;
public:
    VariableGroupDeclarationNode(std::string type, std::vector<PartialVariableDeclarationNode*> vars, bool r_o, bool cust);
    
    ~VariableGroupDeclarationNode();

    /*
     * The type of all variables in this collection.
     */
    std::string type() const;

    /*
     * The list of all partial variable declarations in this group.
     */
    std::vector<PartialVariableDeclarationNode*> partial_variable_group () const;

    /*
     * The list of all variables in this group (in a more useful type).
     */
    std::vector<Symbols::VariableSymbol*> variable_list() const;

    /*
     * Whether or not all the variables are constant.
     */
    bool is_const() const;

    /*
     * Whether or not the variables are struct types.
     */
    bool is_struct() const;

    SyntaxKind kind() const override;
};

}

#endif