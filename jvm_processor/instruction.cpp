#include "instruction.h"
#include "instruction_arguments.h"
#include "jvm_opcode_info.h"

namespace JVMProcessor {

Instruction::Instruction(JVMOpCode op_code, InstructionArgument* argument) :
    m_op_code(op_code),
    m_argument(argument) {}

Instruction::Instruction(JVMOpCode op_code) :
    m_op_code(op_code),
    m_argument(new EmptyInstructionArgument()) {}

JVMOpCode Instruction::op_code() const {
    return m_op_code;
}

InstructionArgument* Instruction::argument() const {
    return m_argument;
}

std::string Instruction::op_code_str() const {
    return OpCodeInfo::op_code_name(m_op_code);
}

std::string Instruction::str() const {
    std::string op = op_code_str();
    if (m_argument->kind() == InstructionArgumentKind::Empty)
        return op;
    return op + " " + m_argument->str();
}

}