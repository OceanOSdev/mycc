#pragma once
#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

namespace Symbols {

enum symbol_kind_e {
    FUNCTION,
    VARIABLE,
    PARAMETER,
    STRUCT,
    TYPE
};
using SymbolKind = enum symbol_kind_e;

class Symbol {
protected:
    std::string m_name;
public:
    Symbol(std::string name);
    virtual ~Symbol() = default;

    /*
     * The name identifier of this symbol.
     */
    std::string name() const;

    /*
     * The type of symbol this is.
     */
    virtual SymbolKind kind() const = 0;

};

}

#endif