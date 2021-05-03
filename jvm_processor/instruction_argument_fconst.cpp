#include "instruction_arguments.h"

namespace JVMProcessor {

FConstInstructionArgument::FConstInstructionArgument(float val) :
    m_fval(val) {}


InstructionArgumentKind FConstInstructionArgument::kind() const {
    return InstructionArgumentKind::Float_Constant;
}

std::string FConstInstructionArgument::str() const {
    return std::to_string(m_fval);
}

}