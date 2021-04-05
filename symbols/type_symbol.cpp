#include "type_symbol.h"

namespace Symbols {

TypeSymbol::TypeSymbol(std::string id, TypeAttribute attributes) :
        m_identifier(id), m_attributes(attributes) {}

TypeSymbol::~TypeSymbol() {}

/*
 * The name of the type.
 */
std::string TypeSymbol::name() const { return m_identifier; }

/*
 * Attributes related to this type.
 */
TypeAttribute TypeSymbol::attributes() const { return m_attributes; }

}