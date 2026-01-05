#include "parameter_symbol.h"
#include "symbol_factory.h"
#include "type_symbol.h"

namespace Symbols {

ParameterSymbol *Factory::parameter(std::string name, const TypeSymbol *type) {
  return new ParameterSymbol(name, type, false, false);
}

ParameterSymbol::ParameterSymbol(std::string n, const TypeSymbol *t,
                                 bool is_arr, bool is_const)
    : Symbol(n), m_type(t), m_is_array(is_arr), m_is_const(is_const) {}

ParameterSymbol::~ParameterSymbol() {}

/*
 * The type information related to this parameter.
 */
const TypeSymbol *ParameterSymbol::type() const { return m_type; }

/*
 * Returns whether this parameter is an arry type.
 */
bool ParameterSymbol::is_array() const { return m_is_array; }

/*
 * Returns whether this parameter is a constant type.
 */
bool ParameterSymbol::is_constant() const { return m_is_const; }

/*
 * The type of symbol this is.
 */
SymbolKind ParameterSymbol::kind() const { return SymbolKind::PARAMETER; }

} // namespace Symbols