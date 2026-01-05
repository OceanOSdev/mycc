#pragma once
#ifndef VARIABLE_SYMBOL_H
#define VARIABLE_SYMBOL_H

#include "symbol.h"
#include <string>

namespace Symbols {
class TypeSymbol;
class ParameterSymbol;
class VariableSymbol : public Symbol {
private:
  const TypeSymbol *m_type;
  bool arr;
  int arr_c;
  bool m_is_const;
  bool m_is_global;

public:
  VariableSymbol(std::string n, const TypeSymbol *t, bool is_arr = false,
                 int arr_size = -1, bool is_const = false,
                 bool is_global = false);
  ~VariableSymbol();

  static VariableSymbol *from_parameter_symbol(ParameterSymbol *symbol);

  /*
   * Returns this variable's type.
   */
  const TypeSymbol *var_type() const;

  /*
   * Whether or not this variable is an array.
   */
  bool is_array() const;

  /*
   * The size allocated to this array (assuming `is_array()` returns true).
   */
  int array_size() const;

  /*
   * Whether or not the type is read only.
   */
  bool is_const() const;

  /*
   * Whether or not this variable is in global scope.
   */
  bool is_global() const;

  /*
   * The type of symbol this is.
   */
  SymbolKind kind() const override;

  bool operator==(const VariableSymbol &o) const;
};

} // namespace Symbols

#endif