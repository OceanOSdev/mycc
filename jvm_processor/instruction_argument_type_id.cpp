#include "instruction_arguments.h"
#include "../symbols/type_symbol.h"

namespace JVMProcessor {

TypeIdInstructionArgument::TypeIdInstructionArgument(const Symbols::TypeSymbol* type) :
    m_type(type) {}

std::string TypeIdInstructionArgument::type_id() const {
    if (Symbols::TypeSymbol::are_types_equivalent(m_type, &Symbols::TypeSymbol::Float))
        return m_type->name();
    return m_type->jasm_str(false);
}

InstructionArgumentKind TypeIdInstructionArgument::kind() const {
    return InstructionArgumentKind::Type_Identifier;
}

std::string TypeIdInstructionArgument::str() const {
    return type_id();
}

}