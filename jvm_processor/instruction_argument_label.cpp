#include "instruction_arguments.h"

namespace JVMProcessor {

InstructionArgumentKind LabelInstructionArgument::kind() const {
    return InstructionArgumentKind::Label;
}

std::string LabelInstructionArgument::str() const {
    return m_label;
}

}