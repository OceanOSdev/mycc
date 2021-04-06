#pragma once
#ifndef SCOPED_SYMBOL_TABLE_H
#define SCOPED_SYMBOL_TABLE_H

#include <unordered_map>
#include <string>

namespace Symbols {

class Symbol;
class TypeSymbol;
class VariableSymbol;
class FunctionSymbol;
class StructSymbol;

class ScopedSymbolTable {
private:
    std::unordered_map<std::string, TypeSymbol*> m_types;
    std::unordered_map<std::string, VariableSymbol*> m_variables;
    std::unordered_map<std::string, FunctionSymbol*> m_functions;
    std::unordered_map<std::string, StructSymbol*> m_structs;

    /*
     * Method that each try_declare_[symboltype]_in_scope calls.
     */
    bool try_declare_symbol_in_scope(Symbol* symbol);
public:
    ScopedSymbolTable();
    ScopedSymbolTable(std::unordered_map<std::string, TypeSymbol*> types,
                      std::unordered_map<std::string, VariableSymbol*> variables,
                      std::unordered_map<std::string, FunctionSymbol*> functions,
                      std::unordered_map<std::string, StructSymbol*> structs);
    
    ~ScopedSymbolTable();
    
    /*
     * Tries to declare a type in the current scope.
     * 
     * Returns true if successful and false if the type
     * has been previously declared in this scope.
     */
    bool try_declare_type_in_scope(TypeSymbol* type);

    /*
     * Tries to declare a variable in the current scope.
     * 
     * Returns true if successful and false if the variable
     * has been previously declared in this scope.
     */
    bool try_declare_variable_in_scope(VariableSymbol* variable);
    bool try_declare_function_in_scope(FunctionSymbol* function);
    bool try_declare_struct_in_scope(StructSymbol* strct);

    bool try_lookup_symbol_in_scope(std::string name, Symbol*& type);
};

}

#endif