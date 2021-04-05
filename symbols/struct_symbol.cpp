#include "struct_symbol.h"
#include "variable_symbol.h"

namespace Symbols {

StructSymbol::StructSymbol(std::string name, std::vector<VariableSymbol*> members) :
    m_name(name), m_members(members) {}

StructSymbol::~StructSymbol() {
    
}

/*
 * Name identifier of this struct type.
 */
std::string StructSymbol::name() const { return m_name; }

/*
 * List of the members of this struct type.
 */
std::vector<VariableSymbol*> StructSymbol::members() const { return m_members; }

}