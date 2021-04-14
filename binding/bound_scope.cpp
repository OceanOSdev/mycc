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

bool BoundScope::try_declare_variable(Symbols::VariableSymbol* variable_symbol) {
    return try_declare_symbol(variable_symbol);
}

bool BoundScope::try_declare_function(Symbols::FunctionSymbol* function_symbol) {
    return try_declare_symbol(function_symbol);
}

bool BoundScope::try_declare_struct(Symbols::StructSymbol* struct_symbol) {
    if (m_structs.contains(struct_symbol->name())) 
        return false;
    
    m_structs[struct_symbol->name()] = struct_symbol;
    return true;
}

bool BoundScope::try_look_up_symbol(std::string identifier, Symbols::Symbol*& symbol) {
    if (m_symbols.contains(identifier)) {
        symbol = m_symbols[identifier];
        return true;
    }
    if (is_root()) return false;
    return m_parent->try_look_up_symbol(identifier, symbol);
}

bool BoundScope::try_look_up_variable(std::string identifier, Symbols::VariableSymbol*& symbol) {
    Symbols::Symbol* sy;
    auto exists = try_look_up_symbol(identifier, sy);
    if (exists && symbol->kind() == Symbols::SymbolKind::VARIABLE) {
        symbol = dynamic_cast<Symbols::VariableSymbol*>(sy);
        return true;
    }
    return false;
}

bool BoundScope::try_look_up_function(std::string identifier, Symbols::FunctionSymbol*& symbol) {
    Symbols::Symbol* sy;
    auto exists = try_look_up_symbol(identifier, sy);
    if (exists && symbol->kind() == Symbols::SymbolKind::FUNCTION) {
        symbol = dynamic_cast<Symbols::FunctionSymbol*>(sy);
        return true;
    }
    return false;
}

bool BoundScope::try_declare_symbol(Symbols::Symbol* symbol) {
    if (m_symbols.contains(symbol->name()))
        return false;
    
    m_symbols[symbol->name()] = symbol;
    return true;
}

bool BoundScope::is_root() const {
    return (m_parent == nullptr);
}


}