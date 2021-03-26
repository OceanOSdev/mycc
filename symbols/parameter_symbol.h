#ifndef PARAMETER_SYMBOL_H
#define PARAMETER_SYMBOL_H

#include <string>
#include "type_symbol.h"

namespace Symbols {

class ParameterSymbol {
private:
    TypeSymbol type; // todo: maybe fix
    std::string name;
    bool is_array;
public:
    ParameterSymbol(TypeSymbol t, std::string n, bool is_arr) :
        type(t), name(n), is_array(is_arr) {}
    
    ~ParameterSymbol() {}
    
    /*
     * Returns a string representation of this parameter's type.
     */
    TypeSymbol var_type() const { return type; }

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