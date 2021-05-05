#include "function_symbol.h"
#include "type_symbol.h"
#include "parameter_symbol.h"
#include <compare>
#include "symbol_factory.h"

namespace Symbols {

FunctionSymbol* Factory::function(std::string name, const TypeSymbol* type) {
    return function(name, type, std::vector<Symbols::ParameterSymbol*>{});
}

FunctionSymbol* Factory::function(std::string name, const TypeSymbol* type, std::vector<ParameterSymbol*> params) {
    return new FunctionSymbol(name, type, params);
}

FunctionSymbol* Factory::function(std::string name, const TypeSymbol* type, std::vector<const TypeSymbol*> param_types) {
    std::vector<ParameterSymbol*> params;
    for (std::size_t i = 0; i < param_types.size(); i ++) {
        std::string param_name = "param_" + std::to_string(i);
        params.push_back(parameter(param_name, param_types[i]));
    }
    return function(name, type, params);
}

FunctionSymbol* Factory::init_function() {
    return function("<init>", &TypeSymbol::Void);
}

FunctionSymbol* Factory::clinit_function() {
    return function("<clinit>", &TypeSymbol::Void);
}

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
    if (m_params.size() != other.m_params.size()) return false;
    using vector_size_t = std::vector<Symbols::ParameterSymbol*>::size_type;
    for (vector_size_t i = 0; i < m_params.size(); i ++) {
        auto type_one = *m_params[i]->type();
        auto type_two = *other.m_params[i]->type();
        if (std::is_neq(type_one <=> type_two))
            return false;
    }
    return true;
}

}