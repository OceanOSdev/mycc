#pragma once
#ifndef VARIABLE_SYMBOL_H
#define VARIABLE_SYMBOL_H

#include <string>
#include "symbol.h"

namespace Symbols {

class TypeSymbol;

class VariableSymbol : public Symbol {
private:
    TypeSymbol* m_type;
    bool arr;
    int arr_c;
    bool m_is_const;
public:
    VariableSymbol(std::string n, TypeSymbol* t, bool is_arr = false, int arr_size = -1, bool is_const = false);
    
    ~VariableSymbol();

    /*
     * Returns this variable's type.
     */
    TypeSymbol* var_type() const;

    /*
     * Whether or not this variable is an array.
     */
    bool is_array() const;

    /*
     * The size allocated to this array (assuming `is_array()` returns true).
     */
    int array_size() const;

    /*
     * Whether or not the type is read only.
     */
    bool is_const() const;

    /*
     * The type of symbol this is.
     */
    SymbolKind kind() const override;
};

}

#endif