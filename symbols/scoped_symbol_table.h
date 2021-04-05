#pragma once
#ifndef SCOPED_SYMBOL_TABLE_H
#define SCOPED_SYMBOL_TABLE_H

#include <vector>

namespace Symbols {

class TypeSymbol;
class VariableSymbol;
class FunctionSymbol;
class StructSymbol;

class ScopedSymbolTable {
private:
    std::vector<TypeSymbol*> m_types;
    std::vector<VariableSymbol*> m_variables;
    std::vector<FunctionSymbol*> m_functions;
    std::vector<StructSymbol*> m_structs;

};

}

#endif