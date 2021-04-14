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

/*
 * Grabs an instance of TypeSymbol if it matches one of the
 * primitive types:
 * 
 * - Char
 * 
 * - Int
 * 
 * - Float
 * 
 * Returns Error otherwise.
 */
const TypeSymbol* TypeSymbol::try_lookup_primitive(std::string ident) {
        if (ident == "char") return &TypeSymbol::Char;
        if (ident == "int") return &TypeSymbol::Int;
        if (ident == "float") return &TypeSymbol::Float;
        return &TypeSymbol::Error;
}

/*
 * Grabs an instance of TypeSymbol if it matches one of the
 * primitive function return types:
 * 
 * - Char
 * 
 * - Int
 * 
 * - Float
 * 
 * - Void
 * 
 * Returns Error otherwise.
 */
const TypeSymbol* TypeSymbol::try_lookup_primitive_function(std::string ident) {
        if (ident == "char") return &TypeSymbol::Char;
        if (ident == "int") return &TypeSymbol::Int;
        if (ident == "float") return &TypeSymbol::Float;
        if (ident == "void") return &TypeSymbol::Void;
        return &TypeSymbol::Error;
}

const TypeSymbol* TypeSymbol::get_wider_type(const TypeSymbol* lhs, const TypeSymbol* rhs) {
        auto order = (*lhs <=> *rhs);
        if (order == 0) return lhs; //arbitrary
        return order < 0 ? rhs : (order > 0) ? lhs : nullptr;
}

const TypeSymbol TypeSymbol::Void = TypeSymbol("void", {false,false,false});
const TypeSymbol TypeSymbol::Char = TypeSymbol("char", {false,true,true});
const TypeSymbol TypeSymbol::Int = TypeSymbol("int", {false,true,true});
const TypeSymbol TypeSymbol::Float =  TypeSymbol("float", {false,false,true});
const TypeSymbol TypeSymbol::Error = TypeSymbol("??", {false,false,false});

}