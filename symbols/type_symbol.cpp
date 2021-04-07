#include "type_symbol.h"

namespace Symbols {

std::partial_ordering TypeAttribute::operator<=>(const TypeAttribute& other) const {
        if (( is_struct && !other.is_struct) || (!is_struct &&  other.is_struct))
                return std::partial_ordering::unordered;
        if (is_integer_type && other.is_numeric_type && !other.is_integer_type)
                return std::partial_ordering::greater;
        if (other.is_integer_type && is_numeric_type && !is_integer_type)
                return std::partial_ordering::less;
        return std::partial_ordering::equivalent;        
}

std::partial_ordering TypeSymbol::operator<=>(const TypeSymbol& other) const {
        auto com = this->attributes() <=> other.attributes();
        if (com != std::partial_ordering::equivalent) return com;
        return (m_name <=> other.m_name);
}


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

const TypeSymbol TypeSymbol::Void = TypeSymbol("void", {false,false,false});
const TypeSymbol TypeSymbol::Char = TypeSymbol("char", {false,true,true});
const TypeSymbol TypeSymbol::Int = TypeSymbol("int", {false,true,true});
const TypeSymbol TypeSymbol::Float =  TypeSymbol("float", {false,false,true});
const TypeSymbol TypeSymbol::Error = TypeSymbol("??", {false,false,false});

}