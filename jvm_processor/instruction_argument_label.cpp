#include "../binding/bound_label_statement_node.h"
#include "instruction_arguments.h"

namespace JVMProcessor {

LabelInstructionArgument::LabelInstructionArgument(Binding::BoundLabel *label)
    : m_label(label) {}

InstructionArgumentKind LabelInstructionArgument::kind() const {
  return InstructionArgumentKind::Label;
}

std::string LabelInstructionArgument::label() const { return m_label->name(); }

Binding::BoundLabel *LabelInstructionArgument::bound_label() const {
  return m_label;
}

std::string LabelInstructionArgument::str() const { return m_label->name(); }

} // namespace JVMProcessor