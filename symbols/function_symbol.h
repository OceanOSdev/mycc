#pragma once
#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H

#include <vector>
#include <string>
#include "symbol.h"

namespace Symbols {
class TypeSymbol;
class ParameterSymbol;

class FunctionSymbol : public Symbol {
private:
    const TypeSymbol* m_type;
    std::vector<ParameterSymbol*> m_params;
public:
    FunctionSymbol(std::string name, const TypeSymbol* type, std::vector<ParameterSymbol*> params);
    ~FunctionSymbol();

    /*
     * The type of value that this function returns.
     */
    const TypeSymbol* type() const;

    /*
     * The list of paramters that this function takes in.
     */
    std::vector<ParameterSymbol*> params() const;

    /*
     * The type of symbol this is.
     */
    SymbolKind kind() const override;
};

}

#endif