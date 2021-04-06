#pragma once
#ifndef STRUCT_SYMBOL_H
#define STRUCT_SYMBOL_H

#include <vector>
#include <string>
#include "symbol.h"

namespace Symbols {
class VariableSymbol;

class StructSymbol : public Symbol {
private:
    std::vector<VariableSymbol*> m_members;
public:
    StructSymbol(std::string name, std::vector<VariableSymbol*> members);
    ~StructSymbol();

    /*
     * List of the members of this struct type.
     */
    std::vector<VariableSymbol*> members() const;

    /*
     * The type of symbol this is.
     */
    SymbolKind kind() const override;
};

}

#endif