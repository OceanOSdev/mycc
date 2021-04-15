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
    int m_line_defined;
public:
    FunctionSymbol(std::string name, const TypeSymbol* type, std::vector<ParameterSymbol*> params, int line_defined = -1);
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
     * The line number where the definition of this function
     * can be found.
     * 
     * Returns -1 if not yet defined (only prototyped).
     */
    int line_defined() const;

    /*
     * Updates this symbol with the line number that the
     * function got defined on.
     */
    void set_line_defined(int line);

    /*
     * The type of symbol this is.
     */
    SymbolKind kind() const override;

    bool operator==(const FunctionSymbol& other);
};

}

#endif