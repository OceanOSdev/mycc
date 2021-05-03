#include "instruction_arguments.h"

namespace JVMProcessor {

IConstInstructionArgument::IConstInstructionArgument(int val) :
    m_ival(val) {}

InstructionArgumentKind IConstInstructionArgument::kind() const {
    return InstructionArgumentKind::Integer_Constant;
}

std::string IConstInstructionArgument::str() const {
    return std::to_string(m_ival);
}

}