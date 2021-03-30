#include "program_node.h"

namespace Syntax {

ProgramNode::ProgramNode(std::vector<TranslationUnitNode*> units) :
    m_units(units) {}

ProgramNode::~ProgramNode() {
    std::vector<TranslationUnitNode*>::iterator tu_iter;
    for (tu_iter = m_units.begin(); tu_iter != m_units.end(); tu_iter++)
        delete *tu_iter;
}

/*
 * Gets the translation units in the program.
 */
std::vector<TranslationUnitNode*> ProgramNode::units() const {
    return m_units;
}

}