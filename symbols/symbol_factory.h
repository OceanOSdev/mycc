#pragma once
#ifndef SYMBOL_FACTORY_H
#define SYMBOL_FACTORY_H

#include <string>
#include <vector>

namespace Symbols {
class TypeSymbol;
class VariableSymbol;
class FunctionSymbol;
class ParameterSymbol;

namespace Factory {

VariableSymbol* char_var(std::string identifier);
VariableSymbol* int_var(std::string identifier);
VariableSymbol* float_var(std::string identifier);
VariableSymbol* string_var(std::string identifier);
VariableSymbol* obj_var(std::string identifier, const TypeSymbol* type);
VariableSymbol* array(VariableSymbol* variable, int array_size);
VariableSymbol* constant(VariableSymbol* variable);

const TypeSymbol* type(std::string identifier);
const TypeSymbol* char_type();
const TypeSymbol* int_type();
const TypeSymbol* float_type();
const TypeSymbol* void_type();

FunctionSymbol* function(std::string name, const TypeSymbol* type);
FunctionSymbol* function(std::string name, const TypeSymbol* type, std::vector<ParameterSymbol*> params);
FunctionSymbol* function(std::string name, const TypeSymbol* type, std::vector<const TypeSymbol*> param_types);

FunctionSymbol* init_function();
FunctionSymbol* clinit_function();

ParameterSymbol* parameter(std::string name, const TypeSymbol* type);

}
}

#endif