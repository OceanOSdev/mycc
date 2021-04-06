#pragma once
#ifndef TYPE_SYMBOL_H
#define TYPE_SYMBOL_H

#include <string>
#include "symbol.h"

namespace Symbols {

using TypeAttribute = struct TypeAttribute;
struct TypeAttribute {
    bool is_struct;
    bool is_integer_type;
    bool is_numeric_type;
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

};

const TypeSymbol PrimativeTypes[4] = {
    TypeSymbol("void", {false,false,false}),
    TypeSymbol("char", {false,true,true}),
    TypeSymbol("int", {false,true,true}),
    TypeSymbol("float", {false,false,true})
};

}

#endif