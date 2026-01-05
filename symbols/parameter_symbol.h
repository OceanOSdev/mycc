#pragma once
#ifndef PARAMETER_SYMBOL_H
#define PARAMETER_SYMBOL_H

#include "symbol.h"
#include <string>

namespace Symbols {

class TypeSymbol;

class ParameterSymbol : public Symbol {
private:
  const TypeSymbol *m_type;
  bool m_is_array;
  bool m_is_const;

public:
  ParameterSymbol(std::string n, const TypeSymbol *t, bool is_arr,
                  bool is_const);

  ~ParameterSymbol();

  /*
   * The type information related to this parameter.
   */
  const TypeSymbol *type() const;

  /*
   * Returns whether this parameter is an arry type.
   */
  bool is_array() const;

  /*
   * Returns whether this parameter is a constant type.
   */
  bool is_constant() const;

  /*
   * The type of symbol this is.
   */
  SymbolKind kind() const override;
};

} // namespace Symbols

#endif