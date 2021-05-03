#include "instruction.h"
#include "instruction_arguments.h"
#include "jvm_opcode_info.h"

namespace JVMProcessor {

Instruction::Instruction(JVMOpCode op_code, std::vector<InstructionArgument*> arguments) :
    m_op_code(op_code),
    m_arguments(arguments) {}

Instruction::Instruction(JVMOpCode op_code, InstructionArgument* argument) :
    m_op_code(op_code),
    m_arguments({argument}) {}

Instruction::Instruction(JVMOpCode op_code) :
    m_op_code(op_code) {}

JVMOpCode Instruction::op_code() const {
    return m_op_code;
}

std::vector<InstructionArgument*> Instruction::arguments() const {
    return m_arguments;
}

std::string Instruction::op_code_str() const {
    return OpCodeInfo::op_code_name(m_op_code);
}

std::string Instruction::str() const {
    std::string op = op_code_str();
    if (m_arguments.empty() || m_arguments[0]->kind() == InstructionArgumentKind::Empty)
        return op;
    std::string args = "";
    for (auto arg : m_arguments) {
        args += " " + arg->str();
    }
    return op + args;
}

}