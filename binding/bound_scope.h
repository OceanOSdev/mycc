#pragma once
#ifndef BOUND_SCOPE_H
#define BOUND_SCOPE_H

#include <string>
#include <unordered_map>

namespace Symbols {
class Symbol;
class TypeSymbol;
class VariableSymbol;
class FunctionSymbol;
class StructSymbol;
} // namespace Symbols

namespace Binding {

class BoundScope {
private:
  BoundScope *m_parent;
  std::unordered_map<std::string, const Symbols::TypeSymbol *> m_types;
  std::unordered_map<std::string, Symbols::StructSymbol *> m_structs;
  std::unordered_map<std::string, Symbols::FunctionSymbol *> m_function_symbols;
  std::unordered_map<std::string, Symbols::VariableSymbol *> m_variable_symbols;

public:
  BoundScope(BoundScope *parent);

  BoundScope *parent() const;
  bool is_root() const;

  bool try_declare_type(const Symbols::TypeSymbol *type_symbol);

  bool try_declare_variable(Symbols::VariableSymbol *variable_symbol);
  bool try_declare_function(Symbols::FunctionSymbol *function_symbol);
  bool try_declare_struct(Symbols::StructSymbol *struct_symbol);

  bool try_look_up_variable(std::string identifier,
                            Symbols::VariableSymbol *&symbol);
  bool try_look_up_function(std::string identifier,
                            Symbols::FunctionSymbol *&symbol);
  bool try_look_up_struct(std::string identifier,
                          Symbols::StructSymbol *&symbol);
  bool try_look_up_type(std::string identifier,
                        const Symbols::TypeSymbol *&symbol);
};
} // namespace Binding

#endif