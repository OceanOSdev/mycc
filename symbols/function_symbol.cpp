#include "function_symbol.h"
#include "type_symbol.h"
#include "parameter_symbol.h"

namespace Symbols {

FunctionSymbol::FunctionSymbol(std::string name, TypeSymbol* type, std::vector<ParameterSymbol*> params) :
    m_name(name), m_type(type), m_params(params) {}

FunctionSymbol::~FunctionSymbol() {

}

/*
 * The name identifier for this function.
 */
std::string FunctionSymbol::name() const { return m_name; }

/*
 * The type of value that this function returns.
 */
TypeSymbol* FunctionSymbol::type() const { return m_type; }

/*
 * The list of paramters that this function takes in.
 */
std::vector<ParameterSymbol*> FunctionSymbol::params() const { return m_params; }

}