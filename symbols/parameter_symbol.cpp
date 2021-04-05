#include "parameter_symbol.h"
#include "type_symbol.h"

namespace Symbols {

ParameterSymbol::ParameterSymbol(TypeSymbol* t, std::string n, bool is_arr) :
    m_type(t), name(n), is_array(is_arr) {}
    
ParameterSymbol::~ParameterSymbol() {}
    
/*
 * The type information related to this parameter.
 */
TypeSymbol* ParameterSymbol::var_type() const { return m_type; }

/*
 * Returns a string representation of this parameter's identifier.
 */
std::string ParameterSymbol::var_name() const { return name; }

/*
 * Returns whether this parameter is an arry type.
 */
bool ParameterSymbol::array() const { return is_array; }


}