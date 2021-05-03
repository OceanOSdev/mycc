/// NOTE: Maybe this doesn't need to be its own file?

#include "instruction_arguments.h"

namespace JVMProcessor {

InstructionArgumentKind EmptyInstructionArgument::kind() const {
    return InstructionArgumentKind::Empty;
}

std::string EmptyInstructionArgument::str() const {
    return "";
}

}