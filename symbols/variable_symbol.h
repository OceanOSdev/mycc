#pragma once
#ifndef VARIABLE_SYMBOL_H
#define VARIABLE_SYMBOL_H

#include <string>

namespace Symbols {

class TypeSymbol;

class VariableSymbol {
private:
    TypeSymbol* m_type;
    std::string name;
    bool arr;
    int arr_c;
    bool m_is_const;
public:
    VariableSymbol(TypeSymbol* t, std::string n, bool is_arr = false, int arr_size = -1, bool is_const = false);
    
    /*
     * Returns this variable's type.
     */
    TypeSymbol* var_type() const;

    /*
     * Returns a string representation of this variable's identifier.
     */
    std::string var_name() const;

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
};

}

#endif