#ifndef SYNTAX_TRANSLATION_UNIT_NODE_H
#define SYNTAX_TRANSLATION_UNIT_NODE_H

#include <vector>
#include "syntax_node.h"
#include "global_declaration_node.h"

namespace Syntax {

class TranslationUnitNode : public SyntaxNode {
private:
    std::vector<GlobalDeclarationNode*> m_global_declarations;
public:
    TranslationUnitNode(std::vector<GlobalDeclarationNode*> glob_decs) :
        m_global_declarations(glob_decs) {}

    ~TranslationUnitNode() {
        std::vector<GlobalDeclarationNode*>::iterator gd_iter;
        for (gd_iter = m_global_declarations.begin(); gd_iter != m_global_declarations.end(); gd_iter++)
            delete *gd_iter;
    }

    /*
     * List of global declarations in this translation unit.
     */
    std::vector<GlobalDeclarationNode*> global_declarations() const { return m_global_declarations; }
};

}

#endif