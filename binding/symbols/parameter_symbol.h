#ifndef BOUND_PARAMETER_SYMBOL_H
#define BOUND_PARAMETER_SYMBOL_H

#include <string>

namespace Binding {

class ParameterSymbol {
private:
    bool is_constant;
    std::string type; // todo: maybe fix
    std::string name;
    bool is_array;
public:
    ParameterSymbol() {}
    ParameterSymbol(bool is_const, std::string t, std::string n, bool is_arr) :
        is_constant(is_const), type(t), name(n), is_array(is_arr) {}
    
    /*
     * Returns whether this parameter is marked as constant.
     */
    bool constant() const { return is_constant; }

    /*
     * Returns a string representation of this parameter's type.
     */
    std::string var_type() const { return type; }

    /*
     * Returns a string representation of this parameter's identifier.
     */
    std::string var_name() const { return name; }

    /*
     * Returns whether this parameter is an arry type.
     */
    bool array() const { return is_array; }
};

}

#endif