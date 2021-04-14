#include "function_symbol.h"
#include "type_symbol.h"
#include "parameter_symbol.h"

namespace Symbols {

FunctionSymbol::FunctionSymbol(std::string name, const TypeSymbol* type, std::vector<ParameterSymbol*> params) :
    Symbol(name), m_type(type), m_params(params) {}

FunctionSymbol::~FunctionSymbol() {

}

/*
 * The type of value that this function returns.
 */
const TypeSymbol* FunctionSymbol::type() const { return m_type; }

/*
 * The list of paramters that this function takes in.
 */
std::vector<ParameterSymbol*> FunctionSymbol::params() const { return m_params; }

/*
 * The type of symbol this is.
 */
SymbolKind FunctionSymbol::kind() const { return SymbolKind::FUNCTION; }

}