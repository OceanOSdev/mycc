#pragma once
#ifndef SYNTAX_STRUCT_DECLARATION_NODE_H
#define SYNTAX_STRUCT_DECLARATION_NODE_H

#include <string>
#include <vector>
#include "local_declaration_node.h"
#include "variable_group_declaration_node.h"

namespace Symbols {
class VariableSymbol;
}
namespace Syntax {
class SyntaxToken;
class StructDeclarationNode : public LocalDeclarationNode {
private:
    std::string name;
    std::vector<VariableGroupDeclarationNode*> members;
    std::vector<Symbols::VariableSymbol*> m_members;
public:
    StructDeclarationNode(SyntaxToken* token, std::string id, std::vector<VariableGroupDeclarationNode*> vars);

    ~StructDeclarationNode();

    /*
     * The name of the struct.
     */
    std::string struct_name() const;

    /*
     * The members (variables) that this struct contains.
     */
    std::vector<VariableGroupDeclarationNode*> struct_members() const;

    /*
     * The members (variables) that this struct contains.
     * 
     * Note: This container uses the VariableSymbol type
     * to describe the members.
     */
    std::vector<Symbols::VariableSymbol*> struct_member_list() const;

    SyntaxKind kind() const override;

};

}

#endif