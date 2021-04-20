#include "variable_symbol.h"
#include "type_symbol.h"
#include "parameter_symbol.h"

namespace Symbols {

VariableSymbol::VariableSymbol(std::string n, const TypeSymbol* t, bool is_arr, int arr_size, bool is_const) :
        Symbol(n), m_type(t), arr(is_arr), arr_c(arr_size), m_is_const(is_const) {}

VariableSymbol::~VariableSymbol() {}

VariableSymbol* VariableSymbol::from_parameter_symbol(ParameterSymbol* symbol) {
        return new VariableSymbol(symbol->name(), symbol->type(), symbol->is_array(), 0, symbol->is_constant());
}

/*
 * Returns this variable's type.
 */
const TypeSymbol* VariableSymbol::var_type() const { return m_type; }

/*
 * Whether or not this variable is an array.
 */
bool VariableSymbol::is_array() const { return arr; }

/*
 * The size allocated to this array (assuming `is_array()` returns true).
 */
int VariableSymbol::array_size() const { return arr_c; }

/*
 * Whether or not the type is read only.
 */
bool VariableSymbol::is_const() const { return m_is_const; }

/*
 * The type of symbol this is.
 */
SymbolKind VariableSymbol::kind() const { return SymbolKind::VARIABLE; }


}