#include "symbol.h"

namespace Symbols {

Symbol::Symbol(std::string name) : m_name(name) {}

/*
 * The name identifier of this symbol.
 */
std::string Symbol::name() const { return m_name; }

}