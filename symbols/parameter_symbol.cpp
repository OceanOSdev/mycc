#include "parameter_symbol.h"
#include "type_symbol.h"

namespace Symbols {

ParameterSymbol::ParameterSymbol(std::string n, TypeSymbol* t, bool is_arr) :
    Symbol(n), m_type(t), is_array(is_arr) {}
    
ParameterSymbol::~ParameterSymbol() {}
    
/*
 * The type information related to this parameter.
 */
TypeSymbol* ParameterSymbol::var_type() const { return m_type; }

/*
 * Returns whether this parameter is an arry type.
 */
bool ParameterSymbol::array() const { return is_array; }

/*
 * The type of symbol this is.
 */
SymbolKind ParameterSymbol::kind() const { return SymbolKind::PARAMETER; }

}