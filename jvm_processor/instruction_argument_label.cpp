#include "instruction_arguments.h"

namespace JVMProcessor {

LabelInstructionArgument::LabelInstructionArgument(std::string label) :
    m_label(label) {}

InstructionArgumentKind LabelInstructionArgument::kind() const {
    return InstructionArgumentKind::Label;
}

std::string LabelInstructionArgument::str() const {
    return m_label;
}

}