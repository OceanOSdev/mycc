#ifndef SYNTAX_VARIABLE_SYMBOL_H
#define SYNTAX_VARIABLE_SYMBOL_H

#include <string>

namespace Syntax {

class VariableSymbol {
private:
    bool is_constant;
    std::string type; // todo: maybe fix
    std::string name;
public:
    VariableSymbol(bool is_const, std::string t, std::string n) :
        is_constant(is_const), type(t), name(n) {}
    
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
};

}

#endif