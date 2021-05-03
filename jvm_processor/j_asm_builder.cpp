#include <stdexcept>

#include "j_asm_builder.h"
#include "builder_state.h"
#include "instruction.h"
#include "instruction_arguments.h"
#include "jvm_opcodes.h"
#include "jvm_opcode_info.h"
#include "finalized_body.h"

namespace JVMProcessor {

FinalizedBody::FinalizedBody(std::vector<std::string> body) :
    m_body(body) {}

std::vector<std::string> FinalizedBody::body() const {
    return m_body;
}

JAsmBuilder::JAsmBuilder() :
    m_builder_state(new BuilderState()) {}

void JAsmBuilder::adjust_stack(int net_change) {
    m_builder_state->adjust_stack(net_change);
}

void JAsmBuilder::emit_op_code(JVMOpCode op_code) {
    emit_op_code(op_code, OpCodeInfo::net_stack_modification(op_code));
}

void JAsmBuilder::emit_op_code(JVMOpCode op_code, int stack_adj) {
    if (OpCodeInfo::required_argument_kind(op_code) != InstructionArgumentKind::Empty)
        throw new std::runtime_error("Invalid number of arguments for op code '" + OpCodeInfo::op_code_name(op_code) + "'");
    
    if (OpCodeInfo::is_control_transfer(op_code))
        throw new std::runtime_error("Do not emit control transferring op codes directly");

    m_builder_state->adjust_stack(stack_adj);
    m_builder_state->add_op_code(op_code);
    m_instructions.push_back(new Instruction(op_code));
}

void JAsmBuilder::emit_op_code(JVMOpCode op_code, InstructionArgument* arg) {
    emit_op_code(op_code, arg, OpCodeInfo::net_stack_modification(op_code));
}

void JAsmBuilder::emit_op_code(JVMOpCode op_code, InstructionArgument* arg, int stack_adj) {
    if (OpCodeInfo::required_argument_kind(op_code) != InstructionArgumentKind::Empty && arg == nullptr)
        throw new std::runtime_error("Invalid number of arguments for op code '" + OpCodeInfo::op_code_name(op_code) + "'");
    
    if (OpCodeInfo::required_argument_kind(op_code) != arg->kind())
        throw new std::runtime_error("Invalid argument for op code '" + OpCodeInfo::op_code_name(op_code) + "'");
    
    if (OpCodeInfo::is_control_transfer(op_code))
        throw new std::runtime_error("Do not emit control transferring op codes directly");

    m_builder_state->adjust_stack(stack_adj);
    m_builder_state->add_op_code(op_code);
    m_instructions.push_back(new Instruction(op_code, arg));
}

void JAsmBuilder::emit_return_op_code(ReturnOpType ret_op_type) {
    if (ret_op_type != ReturnOpType::Void)
        m_builder_state->adjust_stack(-1);
    
    JVMOpCode op_code;
    switch (ret_op_type) {
        case ReturnOpType::Void: op_code = JVMOpCode::_return; break;
        case ReturnOpType::Integer: op_code = JVMOpCode::ireturn; break;
        case ReturnOpType::Float: op_code = JVMOpCode::freturn; break;
        case ReturnOpType::Object: op_code = JVMOpCode::areturn; break;
        default: throw new std::runtime_error("Invalid ret operator type"); // <-- should never happen.
    }

    m_builder_state->add_op_code(op_code);
    m_instructions.push_back(new Instruction(op_code));
}

void JAsmBuilder::emit_branch_op_code(JVMOpCode op_code, InstructionArgument* arg) {
    if (!OpCodeInfo::is_branch(op_code) && op_code != JVMOpCode::nop) 
        throw new std::runtime_error("Op code is not a branching type");
    
    if (arg->kind() != InstructionArgumentKind::Label)
        throw new std::runtime_error("Invalid argument for op code '" + OpCodeInfo::op_code_name(op_code) + "'");
    
    m_builder_state->adjust_stack(OpCodeInfo::net_stack_modification(op_code));
    if (op_code == JVMOpCode::nop) return;

    m_builder_state->add_op_code(op_code);
    m_instructions.push_back(new Instruction(op_code, arg));    
}

std::vector<std::string> JAsmBuilder::current_instruction_listing() const {
    std::vector<std::string> instr_list;

    for (auto instr : m_instructions) 
        instr_list.push_back(instr->str());

    return instr_list;
}

FinalizedBody* JAsmBuilder::finalize() {
    std::vector<std::string> instr_list;

    for (auto instr : m_instructions) 
        instr_list.push_back(instr->str());

    return new FinalizedBody(instr_list);
}

}