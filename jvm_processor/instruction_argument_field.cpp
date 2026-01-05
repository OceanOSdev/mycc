#include "../symbols/struct_symbol.h"
#include "../symbols/symbol_factory.h"
#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"
#include "instruction_arguments.h"

namespace JVMProcessor {

FieldAccessorInstructionArgument::FieldAccessorInstructionArgument(
    Symbols::StructSymbol *_struct, Symbols::VariableSymbol *member)
    : m_struct(_struct), m_member(member) {}

FieldAccessorInstructionArgument::FieldAccessorInstructionArgument(
    std::string filename_base, Symbols::VariableSymbol *member)
    : m_struct(Symbols::Factory::memberless_struct(filename_base)),
      m_member(member) {}

InstructionArgumentKind FieldAccessorInstructionArgument::kind() const {
  return InstructionArgumentKind::Field_Accessor;
}

std::string FieldAccessorInstructionArgument::encapsulating_type_id() const {
  return m_struct->name();
}

std::string FieldAccessorInstructionArgument::member_id() const {
  return m_member->name();
}

std::string FieldAccessorInstructionArgument::member_type_id() const {
  return m_member->var_type()->jasm_str();
}

std::string FieldAccessorInstructionArgument::str() const {
  return "Field " + encapsulating_type_id() + " " + member_id() + " " +
         member_type_id();
}

} // namespace JVMProcessor