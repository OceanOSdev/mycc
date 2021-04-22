#include "type_symbol.h"

namespace Symbols {

std::partial_ordering TypeAttribute::operator<=>(const TypeAttribute& other) const {
        if ((is_struct && !other.is_struct) || (!is_struct && other.is_struct) ||
            (is_array && !other.is_array) || (!is_array && other.is_array))
                return std::partial_ordering::unordered;
        if (is_integer_type && other.is_numeric_type && !other.is_integer_type)
                return std::partial_ordering::less;
        if (other.is_integer_type && is_numeric_type && !is_integer_type)
                return std::partial_ordering::greater;
        if (is_integer_type == other.is_integer_type &&
            is_numeric_type == other.is_numeric_type &&
            is_struct == other.is_struct &&
            is_array == other.is_array)
        return std::partial_ordering::equivalent;

        return std::partial_ordering::unordered;    
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
 * Returns a copy of the current type symbol
 * but with the array attribute true.
 */
const TypeSymbol* TypeSymbol::as_array_type() const {
        TypeAttribute attributes = m_attributes;
        attributes.is_array = true;
        return new TypeSymbol(name(), attributes);
}

/*
 * Returns a copy of the current type symbol
 * but with the array attribute false.
 * 
 * Used mostly just for grabbing the type of an array
 * item in a bound index expression
 */
const TypeSymbol* TypeSymbol::as_array_element_type() const {
        TypeAttribute attributes = m_attributes;
        attributes.is_array = false;
        return new TypeSymbol(name(), attributes);
}

/*
 * Returns a copy of the current type symbol
 * but with the const attribute true.
 */
const TypeSymbol* TypeSymbol::as_const_type() const {
        TypeAttribute attributes = m_attributes;
        attributes.is_const = true;
        return new TypeSymbol(name(), attributes);
}

/*
 * Returns a copy of the current type symbol
 * but with the const attribute false.
 */
const TypeSymbol* TypeSymbol::as_mutable_type() const {
        TypeAttribute attributes = m_attributes;
        attributes.is_const = false;
        return new TypeSymbol(name(), attributes);
}

/*
 * The type of symbol this is.
 */
SymbolKind TypeSymbol::kind() const { return SymbolKind::TYPE; }

/*
 * String representing type including attributes.
 */
std::string TypeSymbol::str() const {
        std::string ret = "";
        if (m_attributes.is_const) ret += "const ";
        if (m_attributes.is_struct) ret += "struct ";
        ret = ret + name();
        if (m_attributes.is_array) ret += "[]";
        return ret;
}

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

/*
 * Whether the given type can be used as a conditional type in
 * an if statement, for loop, while loop, do while, or ternary
 * expression.
 */
bool TypeSymbol::is_conditional_type(const TypeSymbol* type) {
        if (type == nullptr) return false;
        return type->attributes().is_numeric_type && !type->attributes().is_array;
}

/*
 * Determines whether the lhs type is the same as the rhs type
 * 
 * OR
 * 
 * whether the lhs type can be implicitly widened to the rhs type. 
 */
bool TypeSymbol::are_types_equivalent(const TypeSymbol* lhs, const TypeSymbol* rhs) {
        if (lhs->attributes().is_array && rhs->attributes().is_array) {
                return lhs->name() == rhs->name();
        }
        return std::is_lteq(*lhs <=> *rhs);
}

/*
 * Determines whether the lhs type is the same as the rhs type.
 */
bool TypeSymbol::are_types_equal(const TypeSymbol* lhs, const TypeSymbol* rhs) {
        return std::is_eq(*lhs <=> *rhs);
}

/*
 * Whether or not we will need to explicitly cast from type "from" to type "to"
 * when we generate bytecode.
 * 
 * Should be used while rewriting the bound tree to deal with cases where an
 * integer type needs to be widened to a non-integer numeric type.
 */
bool TypeSymbol::requires_bytecode_cast(const TypeSymbol* from, const TypeSymbol* to) {
        if (from->attributes().is_array && to->attributes().is_array)
                return (from->name() != to->name()); //if they're same base type, don't need to cast, otherwise need (invalid) cast

        // Edge case, either <from>, <to>, or both are non numeric types
        // If I recall correctly, come rewrite time this should never be
        // the case since I believe I have binder throw a type error,
        // but for debug purposes I'll add this in here and remove it
        // later when I feel more confident that we never hit this condition
        if (!(from->attributes().is_numeric_type && to->attributes().is_numeric_type)) {
                return true; // *Technically* would require cast, but such a cast would be invalid anyways 
        }

        return (from->attributes().is_integer_type && !to->attributes().is_integer_type);
}

bool TypeSymbol::is_error_type(const TypeSymbol* type) {
        return are_types_equivalent(type, &TypeSymbol::Error);
}

bool TypeSymbol::is_error_or_incomplete_type(const TypeSymbol* type) {
        if (is_error_type(type)) return true;

        return are_types_equivalent(type, &TypeSymbol::Void);
}


bool TypeSymbol::can_be_explicitly_casted(const TypeSymbol* from, const TypeSymbol* to) {
        auto order = (*from <=> *to);
        return std::is_lteq(order) || std::is_gteq(order);
}

const TypeSymbol TypeSymbol::Void = TypeSymbol("void", {false,false,false});
const TypeSymbol TypeSymbol::Char = TypeSymbol("char", {false,true,true});
const TypeSymbol TypeSymbol::Int = TypeSymbol("int", {false,true,true});
const TypeSymbol TypeSymbol::Float =  TypeSymbol("float", {false,false,true});
const TypeSymbol TypeSymbol::String = TypeSymbol("char", {false,true,true,true});
const TypeSymbol TypeSymbol::Error = TypeSymbol("??", {false,false,false});

}