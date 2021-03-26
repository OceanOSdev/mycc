#ifndef VARIABLE_SYMBOL_H
#define VARIABLE_SYMBOL_H

#include <string>
#include "type_symbol.h"

namespace Symbols {

class VariableSymbol {
private:
    TypeSymbol type;
    std::string name;
    bool arr;
    int arr_c;
public:
    VariableSymbol(TypeSymbol t, std::string n, bool is_arr = false, int arr_size = -1) :
        type(t), name(n), arr(is_arr), arr_c(arr_size) {}
    
    /*
     * Returns this variable's type.
     */
    TypeSymbol var_type() const { return type; }

    /*
     * Returns a string representation of this variable's identifier.
     */
    std::string var_name() const { return name; }

    /*
     * Whether or not this variable is an array.
     */
    bool is_array() const { return arr; }

    /*
     * The size allocated to this array (assuming `is_array()` returns true).
     */
    int array_size() const { return arr_c; }
};

}

#endif