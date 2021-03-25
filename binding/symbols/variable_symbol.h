#ifndef BOUND_VARIABLE_SYMBOL_H
#define BOUND_VARIABLE_SYMBOL_H

#include <string>

namespace Binding {

class VariableSymbol {
private:
    bool is_constant;
    std::string type; // todo: maybe fix
    std::string name;
    bool is_array;
    int arr_length;
public:
    VariableSymbol(bool is_const, std::string t, std::string n, bool is_arr, int arr_l) :
        is_constant(is_const), type(t), name(n), is_array(is_arr), arr_length(arr_l) {}
    
    /*
     * Returns whether this variable is marked as constant.
     */
    bool constant() const { return is_constant; }

    /*
     * Returns a string representation of this variable's type.
     */
    std::string var_type() const { return type; }

    /*
     * Returns a string representation of this variable's identifier.
     */
    std::string var_name() const { return name; }

    /*
     * Returns whether this variable is an arry type.
     */
    bool array() const { return is_array; }

    /*
     * Returns the size allocated to this variable 
     * if it is an array type.
     * 
     * Returns -1 otherwise
     */

    int size() const { return is_array ? arr_length : -1; }
};

}

#endif