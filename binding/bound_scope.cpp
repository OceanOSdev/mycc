#include "bound_scope.h"
#include "../symbols/type_symbol.h"
#include "../symbols/function_symbol.h"
#include "../symbols/variable_symbol.h"
#include "../symbols/struct_symbol.h"

namespace Binding {

BoundScope::BoundScope(BoundScope* parent) : m_parent(parent) {}

BoundScope* BoundScope::parent() const {
    return m_parent;
}

bool BoundScope::try_declare_type(const Symbols::TypeSymbol* type_symbol) {
    if (m_types.contains(type_symbol->name()))
        return false;
    
    m_types[type_symbol->name()] = type_symbol;
    return true;
}

bool BoundScope::try_declare_variable(Symbols::VariableSymbol* variable_symbol) {
    if (m_variable_symbols.contains(variable_symbol->name()))
        return false;
    m_variable_symbols[variable_symbol->name()] = variable_symbol;
    return true;
}

bool BoundScope::try_declare_function(Symbols::FunctionSymbol* function_symbol) {
    if (m_function_symbols.contains(function_symbol->name())) {
        // if the function exists in scope already, check if what we
        // are trying to declare has is the same function up to parameter
        // names, in which case we can report back successful.
        auto new_func = dynamic_cast<Symbols::FunctionSymbol*>(function_symbol);
        auto orig_func = dynamic_cast<Symbols::FunctionSymbol*>(m_function_symbols[function_symbol->name()]);
        return (*new_func == *orig_func);
    }
    
    m_function_symbols[function_symbol->name()] = function_symbol;
    return true;
}

bool BoundScope::try_declare_struct(Symbols::StructSymbol* struct_symbol) {
    if (m_structs.contains(struct_symbol->name())) 
        return false;
    
    m_structs[struct_symbol->name()] = struct_symbol;
    return true;
}

bool BoundScope::try_look_up_variable(std::string identifier, Symbols::VariableSymbol*& symbol) {
    if (m_variable_symbols.contains(identifier)) {
        symbol = m_variable_symbols[identifier];
        return true;
    }
    if (is_root()) return false;
    return m_parent->try_look_up_variable(identifier, symbol);
}

bool BoundScope::try_look_up_struct(std::string identifier, Symbols::StructSymbol*& symbol) {
    if (m_structs.contains(identifier)) {
        symbol = m_structs[identifier];
        return true;
    }

    if (is_root()) return false;
    return m_parent->try_look_up_struct(identifier, symbol);
}

bool BoundScope::try_look_up_function(std::string identifier, Symbols::FunctionSymbol*& symbol) {
    if (m_function_symbols.contains(identifier)) {
        symbol = m_function_symbols[identifier];
        return true;
    }
    if (is_root()) return false;
    return m_parent->try_look_up_function(identifier, symbol);
}

bool BoundScope::try_look_up_type(std::string identifier, const Symbols::TypeSymbol*& symbol) {
    if (m_types.contains(identifier)) {
        symbol = m_types[identifier];
        return true;
    }
    if (is_root()) return false;
    return m_parent->try_look_up_type(identifier, symbol);
}


bool BoundScope::is_root() const {
    return (m_parent == nullptr);
}


}