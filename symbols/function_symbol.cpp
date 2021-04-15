#include "function_symbol.h"
#include "type_symbol.h"
#include "parameter_symbol.h"
#include <compare>

namespace Symbols {

FunctionSymbol::FunctionSymbol(std::string name, const TypeSymbol* type, std::vector<ParameterSymbol*> params, int line_defined) :
    Symbol(name), m_type(type), m_params(params), m_line_defined(line_defined) {}

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
 * The line number where the definition of this function
 * can be found.
 * 
 * Returns -1 if not yet defined (only prototyped).
 */
int FunctionSymbol::line_defined() const {
    return m_line_defined;
}

/*
 * Updates this symbol with the line number that the
 * function got defined on.
 */
void FunctionSymbol::set_line_defined(int line) {
    m_line_defined = line;
}

/*
 * The type of symbol this is.
 */
SymbolKind FunctionSymbol::kind() const { return SymbolKind::FUNCTION; }

bool FunctionSymbol::operator==(const FunctionSymbol& other) {
    if (std::is_neq(*m_type <=> *other.m_type)) return false;
    if (m_name != other.m_name) return false;
    if (m_params.size() != other.m_name.size()) return false;
    using vector_size_t = std::vector<Symbols::ParameterSymbol*>::size_type;
    for (vector_size_t i = 0; i < m_params.size(); i ++) {
        if (std::is_neq(*m_params[i]->type() <=> *other.m_params[i]->type()))
            return false;
    }
    return true;
}

}