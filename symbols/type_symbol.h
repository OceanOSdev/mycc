#pragma once
#ifndef TYPE_SYMBOL_H
#define TYPE_SYMBOL_H

#include <string>

namespace Symbols {

using TypeAttribute = struct TypeAttribute;
struct TypeAttribute {
    bool is_struct;
    bool is_integer_type;
    bool is_numeric_type;
};

class TypeSymbol {
private:
    std::string m_identifier;
    TypeAttribute m_attributes;
public:
    TypeSymbol(std::string id, TypeAttribute attributes);

    ~TypeSymbol();

    /*
     * The name of the type.
     */
    std::string name() const;

    /*
     * Attributes related to this type.
     */
    TypeAttribute attributes() const;

};

const TypeSymbol PrimativeTypes[4] = {
    TypeSymbol("void", {false,false,false}),
    TypeSymbol("char", {false,true,true}),
    TypeSymbol("int", {false,true,true}),
    TypeSymbol("float", {false,false,true})
};

}

#endif