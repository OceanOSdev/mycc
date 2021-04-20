#pragma once
#ifndef SYNTAX_TRANSLATION_UNIT_NODE_H
#define SYNTAX_TRANSLATION_UNIT_NODE_H

#include <vector>
#include "syntax_node.h"
#include "global_declaration_node.h"

namespace Syntax {
class SyntaxToken;
class TranslationUnitNode : public SyntaxNode {
private:
    std::vector<GlobalDeclarationNode*> m_global_declarations;
public:
    TranslationUnitNode(SyntaxToken* token, std::vector<GlobalDeclarationNode*> glob_decs);

    ~TranslationUnitNode();

    /*
     * List of global declarations in this translation unit.
     */
    std::vector<GlobalDeclarationNode*> global_declarations() const;

    SyntaxKind kind() const override;
};

}

#endif