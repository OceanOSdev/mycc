#include "variable_symbol.h"
#include "type_symbol.h"
#include "parameter_symbol.h"
#include "symbol_factory.h"

namespace Symbols {

VariableSymbol::VariableSymbol(std::string n, const TypeSymbol* t, bool is_arr, int arr_size, bool is_const, bool is_global) :
        Symbol(n), m_type(t), arr(is_arr), arr_c(arr_size), m_is_const(is_const), m_is_global(is_global) {}

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

bool VariableSymbol::is_global() const { return m_is_global; }

/*
 * The type of symbol this is.
 */
SymbolKind VariableSymbol::kind() const { return SymbolKind::VARIABLE; }




VariableSymbol* Factory::char_var(std::string identifier) {
        return new VariableSymbol(identifier, &TypeSymbol::Char);
}

VariableSymbol* Factory::int_var(std::string identifier) {
        return new VariableSymbol(identifier, &TypeSymbol::Int);
}

VariableSymbol* Factory::float_var(std::string identifier) {
        return new VariableSymbol(identifier, &TypeSymbol::Float);
}

VariableSymbol* Factory::string_var(std::string identifier) {
        return new VariableSymbol(identifier, &TypeSymbol::String);
}

VariableSymbol* Factory::obj_var(std::string identifier, const TypeSymbol* type) {
        return new VariableSymbol(identifier, type);
}

VariableSymbol* Factory::array(VariableSymbol* variable, int array_size) {
        auto old_type = variable->var_type();
        auto new_type = old_type->as_array_type();
        return new VariableSymbol(variable->name(), new_type, true, array_size);
}

VariableSymbol* Factory::constant(VariableSymbol* variable) {
        auto old_type = variable->var_type();
        auto new_type = old_type->as_const_type();
        return new VariableSymbol(variable->name(), new_type, false, -1, true);
}


}