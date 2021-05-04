#include <stdexcept>
#include <cassert>

#include "../symbols/variable_symbol.h"
#include "../symbols/type_symbol.h"

#include "j_asm_builder.h"
#include "builder_state.h"
#include "instruction.h"
#include "instruction_arguments.h"
#include "jvm_opcodes.h"
#include "jvm_opcode_info.h"
#include "finalized_body.h"

namespace JVMProcessor {

// ====================
//    Finalized Body
// ====================

FinalizedBody::FinalizedBody(std::vector<std::string> body, int locals, int max_stack) :
    m_body(body),
    m_local_count(locals),
    m_max_stack(max_stack) {}

std::vector<std::string> FinalizedBody::body() const {
    return m_body;
}

int FinalizedBody::local_count() const {
    return m_local_count;
}

int FinalizedBody::max_stack_size() const {
    return m_max_stack;
}

// ====================
//     JAsmBuilder
// ====================

JAsmBuilder::JAsmBuilder() :
    m_builder_state(new BuilderState()),
    m_finalized(false) {}

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


void JAsmBuilder::emit_local_load(Symbols::VariableSymbol* local) {
    int local_index = get_local_variable_index(local);
    assert(((local_index >= 0), "Local variable is not declared in current scope, cannot emit load."));

    using namespace Symbols;
    if (TypeSymbol::is_bytecode_reference_type(local->var_type()))
        emit_local_load_or_store_reference(local_index);
    else if (TypeSymbol::are_types_equivalent(local->var_type(), &TypeSymbol::Int))
        emit_local_load_or_store_int(local_index);
    else
        emit_local_load_or_store_float(local_index); 
}

void JAsmBuilder::emit_local_store(Symbols::VariableSymbol* local) {
    int local_index = get_local_variable_index(local);
    assert(((local_index >= 0), "Local variable is not declared in current scope, cannot emit store."));

    using namespace Symbols;
    if (TypeSymbol::is_bytecode_reference_type(local->var_type()))
        emit_local_load_or_store_reference(local_index, LSFlag::Store);
    else if (TypeSymbol::are_types_equivalent(local->var_type(), &TypeSymbol::Int))
        emit_local_load_or_store_int(local_index, LSFlag::Store);
    else
        emit_local_load_or_store_float(local_index, LSFlag::Store);
}

void JAsmBuilder::emit_local_load_or_store_int(int local_index, LSFlag flag) {
    bool _emit_load = flag == LSFlag::Load;
    switch (local_index) {
        case 0: emit_op_code(_emit_load ? JVMOpCode::iload_0 : JVMOpCode::istore_0);break;
        case 1: emit_op_code(_emit_load ? JVMOpCode::iload_1 : JVMOpCode::istore_1);break;
        case 2: emit_op_code(_emit_load ? JVMOpCode::iload_2 : JVMOpCode::istore_2);break;
        case 3: emit_op_code(_emit_load ? JVMOpCode::iload_3 : JVMOpCode::istore_3);break;
        default:
            emit_op_code(_emit_load ? JVMOpCode::iload : JVMOpCode::istore, new IConstInstructionArgument(local_index));
    }
}

void JAsmBuilder::emit_local_load_or_store_float(int local_index, LSFlag flag) {
    bool _emit_load = flag == LSFlag::Load;
    switch (local_index) {
        case 0: emit_op_code(_emit_load ? JVMOpCode::fload_0 : JVMOpCode::fstore_0); break;
        case 1: emit_op_code(_emit_load ? JVMOpCode::fload_1 : JVMOpCode::fstore_1); break;
        case 2: emit_op_code(_emit_load ? JVMOpCode::fload_2 : JVMOpCode::fstore_2); break;
        case 3: emit_op_code(_emit_load ? JVMOpCode::fload_3 : JVMOpCode::fstore_3); break;
        default:
            emit_op_code(_emit_load ? JVMOpCode::fload : JVMOpCode::fstore, new IConstInstructionArgument(local_index));
    }
}

void JAsmBuilder::emit_local_load_or_store_reference(int local_index, LSFlag flag) {
    bool _emit_load = flag == LSFlag::Load;
    switch (local_index) {
        case 0: emit_op_code(_emit_load ? JVMOpCode::aload_0 : JVMOpCode::astore_0); break;
        case 1: emit_op_code(_emit_load ? JVMOpCode::aload_1 : JVMOpCode::astore_1); break;
        case 2: emit_op_code(_emit_load ? JVMOpCode::aload_2 : JVMOpCode::astore_2); break;
        case 3: emit_op_code(_emit_load ? JVMOpCode::aload_3 : JVMOpCode::astore_3); break;
        default:
            emit_op_code(_emit_load ? JVMOpCode::aload : JVMOpCode::astore, new IConstInstructionArgument(local_index));
    }
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

    return new FinalizedBody(instr_list, m_locals.size(), m_builder_state->max_stack_size());
}

int JAsmBuilder::max_stack_size() const {
    /* We could throw an exception if we haven't finalized
     * the instruction list yet, but that really wouldn't be too
     * necessary since all it would do is possibly give the wrong
     * value.
     * 
     * Maybe just write a warning to a log if we're in a 
     * debug build?
     */

    return m_builder_state->max_stack_size();
}

void JAsmBuilder::declare_local(Symbols::VariableSymbol* local_variable) {
    // Any errors that would occur due to this declaration
    // should already have been caught by the binder, so
    // we can just directly add the local to the list.
    m_locals.push_back(local_variable);
}


int JAsmBuilder::get_local_variable_index(Symbols::VariableSymbol* local) {
    for (std::size_t i = 0; i < m_locals.size(); i ++) {
        if (m_locals[i] == local) return (int)i;
    }
    return -1;
}
}