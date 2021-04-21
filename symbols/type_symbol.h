#pragma once
#ifndef TYPE_SYMBOL_H
#define TYPE_SYMBOL_H

#include <string>
#include <compare>
#include "symbol.h"

namespace Symbols {

using TypeAttribute = struct TypeAttribute;
struct TypeAttribute {
    bool is_struct = false;
    bool is_integer_type = false;
    bool is_numeric_type = false;
    bool is_array = false;
    bool is_const = false;
    std::partial_ordering operator<=>(const TypeAttribute& other) const;
};

class TypeSymbol : public Symbol{
private:
    TypeAttribute m_attributes;
public:
    TypeSymbol(std::string id, TypeAttribute attributes);

    ~TypeSymbol();

    /*
     * Attributes related to this type.
     */
    TypeAttribute attributes() const;

    /*
     * The type of symbol this is.
     */
    SymbolKind kind() const override;

    /*
     * Returns a copy of the current type symbol
     * but with the array attribute true.
     */
    const TypeSymbol* as_array_type() const;

    /*
     * Returns a copy of the current type symbol
     * but with the array attribute false.
     * 
     * Used mostly just for grabbing the type of an array
     * item in a bound index expression
     */
    const TypeSymbol* as_array_element_type() const;

    /*
     * Returns a copy of the current type symbol
     * but with the const attribute true.
     */
    const TypeSymbol* as_const_type() const;

    /*
     * Returns a copy of the current type symbol
     * but with the const attribute false.
     */
    const TypeSymbol* as_mutable_type() const;

    std::partial_ordering operator<=>(const TypeSymbol& other) const;

    /*
     * String representing type including attributes.
     */
    std::string str() const;

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
    static const TypeSymbol* try_lookup_primitive(std::string ident);
    static const TypeSymbol* try_lookup_primitive_function(std::string ident);
    static const TypeSymbol Void;
    static const TypeSymbol Char;
    static const TypeSymbol Int;
    static const TypeSymbol Float;
    static const TypeSymbol String;
    static const TypeSymbol Error;
    static const TypeSymbol* get_wider_type(const TypeSymbol* lhs, const TypeSymbol* rhs);

    /*
     * Determines whether the lhs type is the same as the rhs type
     * 
     * OR
     * 
     * whether the lhs type can be implicitly widened to the rhs type. 
     */
    static bool are_types_equivalent(const TypeSymbol* lhs, const TypeSymbol* rhs);
    static bool is_error_type(const TypeSymbol* type);
    static bool is_error_or_incomplete_type(const TypeSymbol* type);
    /*
     * Whether the given type can be used as a conditional type in
     * an if statement, for loop, while loop, do while, or ternary
     * expression.
     */
    static bool is_conditional_type(const TypeSymbol* type);
    static bool can_be_explicitly_casted(const TypeSymbol* from, const TypeSymbol* to);
};

}

#endif