#pragma once
#ifndef SYNTAX_PROGRAM_NODE_H
#define SYNTAX_PROGRAM_NODE_H

#include "syntax_node.h"
#include "translation_unit_node.h"
#include <vector>
namespace Syntax {
class SyntaxToken;
class ProgramNode : public SyntaxNode {
private:
    std::vector<TranslationUnitNode*> m_units;
public:
    ProgramNode(SyntaxToken* token, std::vector<TranslationUnitNode*> units);
    ~ProgramNode();

    /*
     * Gets the translation units in the program.
     */
    std::vector<TranslationUnitNode*> units() const;

    SyntaxKind kind() const override;
};

}

#endif