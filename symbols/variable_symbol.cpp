#include "variable_symbol.h"
#include "type_symbol.h"

namespace Symbols {

VariableSymbol::VariableSymbol(TypeSymbol* t, std::string n, bool is_arr, int arr_size, bool is_const) :
        m_type(t), name(n), arr(is_arr), arr_c(arr_size), m_is_const(is_const) {}

/*
 * Returns this variable's type.
 */
TypeSymbol* VariableSymbol::var_type() const { return m_type; }

/*
 * Returns a string representation of this variable's identifier.
 */
std::string VariableSymbol::var_name() const { return name; }

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


}