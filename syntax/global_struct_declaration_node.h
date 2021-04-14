#pragma once
#ifndef SYNTAX_GLOBAL_STRUCT_DECLARATION_NODE_H
#define SYNTAX_GLOBAL_STRUCT_DECLARATION_NODE_H

#include "global_declaration_node.h"
#include "struct_declaration_node.h"

namespace Syntax {

class GlobalStructDeclarationNode : public GlobalDeclarationNode {
private:
    StructDeclarationNode* m_struct_declaration;
public:
    GlobalStructDeclarationNode(StructDeclarationNode* struct_dec);
    
    ~GlobalStructDeclarationNode();

    /*
     * The struct declaration.
     */
    StructDeclarationNode* struct_declaration() const;

    SyntaxKind kind() const override;
};

}

#endif