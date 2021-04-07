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

    std::partial_ordering operator<=>(const TypeSymbol& other) const;

    static const TypeSymbol Void;
    static const TypeSymbol Char;
    static const TypeSymbol Int;
    static const TypeSymbol Float;
    static const TypeSymbol Error;

};

}

#endif