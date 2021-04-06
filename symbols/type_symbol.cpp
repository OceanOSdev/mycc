#include "type_symbol.h"

namespace Symbols {

TypeSymbol::TypeSymbol(std::string id, TypeAttribute attributes) :
        Symbol(id), m_attributes(attributes) {}

TypeSymbol::~TypeSymbol() {}

/*
 * Attributes related to this type.
 */
TypeAttribute TypeSymbol::attributes() const { return m_attributes; }

/*
 * The type of symbol this is.
 */
SymbolKind TypeSymbol::kind() const { return SymbolKind::TYPE; }

}