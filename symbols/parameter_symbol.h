#pragma once
#ifndef PARAMETER_SYMBOL_H
#define PARAMETER_SYMBOL_H

#include <string>

namespace Symbols {

class TypeSymbol;

class ParameterSymbol {
private:
    TypeSymbol* m_type;
    std::string name;
    bool is_array;
public:
    ParameterSymbol(TypeSymbol* t, std::string n, bool is_arr);
    
    ~ParameterSymbol();
    
    /*
     * The type information related to this parameter.
     */
    TypeSymbol* var_type() const;

    /*
     * Returns a string representation of this parameter's identifier.
     */
    std::string var_name() const;

    /*
     * Returns whether this parameter is an arry type.
     */
    bool array() const;
};

}

#endif