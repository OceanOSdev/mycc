#pragma once
#ifndef PARAMETER_SYMBOL_H
#define PARAMETER_SYMBOL_H

#include <string>
#include "symbol.h"

namespace Symbols {

class TypeSymbol;

class ParameterSymbol : public Symbol {
private:
    TypeSymbol* m_type;
    bool is_array;
public:
    ParameterSymbol(std::string n, TypeSymbol* t, bool is_arr);
    
    ~ParameterSymbol();
    
    /*
     * The type information related to this parameter.
     */
    TypeSymbol* var_type() const;

    /*
     * Returns whether this parameter is an arry type.
     */
    bool array() const;

    /*
     * The type of symbol this is.
     */
    SymbolKind kind() const override;
};

}

#endif