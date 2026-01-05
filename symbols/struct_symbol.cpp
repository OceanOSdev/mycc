#include "struct_symbol.h"
#include "symbol_factory.h"
#include "variable_symbol.h"

namespace Symbols {

StructSymbol *Factory::memberless_struct(std::string name) {
  return new StructSymbol(name, {});
}

StructSymbol::StructSymbol(std::string name,
                           std::vector<VariableSymbol *> members)
    : Symbol(name), m_members(members) {}

StructSymbol::~StructSymbol() {}

/*
 * List of the members of this struct type.
 */
std::vector<VariableSymbol *> StructSymbol::members() const {
  return m_members;
}

/*
 * The type of symbol this is.
 */
SymbolKind StructSymbol::kind() const { return SymbolKind::STRUCT; }

} // namespace Symbols