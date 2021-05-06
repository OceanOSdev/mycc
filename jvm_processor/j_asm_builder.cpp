#include <stdexcept>
#include <cassert>

#include "../symbols/variable_symbol.h"
#include "../symbols/type_symbol.h"

#include "../binding/bound_label_statement_node.h"

#include "j_asm_builder.h"
#include "builder_state.h"
#include "instruction.h"
#include "instruction_arguments.h"
#include "jvm_opcodes.h"
#include "jvm_opcode_info.h"
#include "finalized_body.h"
#include "label_fix_up_data.h"

namespace JVMProcessor {

// ====================
//    Finalized Body
// ====================

FinalizedBody::FinalizedBody(std::vector<Instruction*> body, int locals, int max_stack) :
    m_body(body),
    m_local_count(locals),
    m_max_stack(max_stack),
    m_is_hard_coded(false) {}

FinalizedBody::FinalizedBody(std::vector<std::string> body, int locals, int max_stack) :
    m_hard_coded_body(body),
    m_local_count(locals),
    m_max_stack(max_stack),
    m_is_hard_coded(true) {}

std::vector<Instruction*> FinalizedBody::body() const {
    return m_body;
}

std::vector<std::string> FinalizedBody::hard_coded_body() const {
    return m_hard_coded_body;
}

bool FinalizedBody::is_hard_coded() const {
    return m_is_hard_coded;
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
    record_instruction(new Instruction(op_code));
    //m_builder_state->add_op_code(op_code);
    //m_instructions.push_back(new Instruction(op_code));
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
    record_instruction(new Instruction(op_code, arg));
    //m_builder_state->add_op_code(op_code);
    //m_instructions.push_back(new Instruction(op_code, arg));
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

    record_instruction(new Instruction(op_code));
    //m_builder_state->add_op_code(op_code);
    //m_instructions.push_back(new Instruction(op_code));
}

void JAsmBuilder::emit_branch_op_code(JVMOpCode op_code, InstructionArgument* arg) {
    if (!OpCodeInfo::is_branch(op_code) && op_code != JVMOpCode::nop) 
        throw new std::runtime_error("Op code is not a branching type");
    
    if (arg->kind() != InstructionArgumentKind::Label)
        throw new std::runtime_error("Invalid argument for op code '" + OpCodeInfo::op_code_name(op_code) + "'");
    
    m_builder_state->adjust_stack(OpCodeInfo::net_stack_modification(op_code));
    if (op_code == JVMOpCode::nop) return;

    auto label_arg = dynamic_cast<LabelInstructionArgument*>(arg);
    mark_branch_for_fix_up(m_builder_state->op_codes_emitted(), label_arg->bound_label());
    record_instruction(new Instruction(op_code, arg));
    //m_builder_state->add_op_code(op_code);
    //m_instructions.push_back(new Instruction(op_code, arg));    
}

void JAsmBuilder::emit_constant(int arg) {
    JVMOpCode op_code;
    if (-1 <= arg && arg <= 5) {
        uint8_t offset = (uint8_t)JVMOpCode::iconst_0;
        offset += (int8_t)arg;
        op_code = static_cast<JVMOpCode>(offset);
        emit_op_code(op_code);
    } else {
        op_code = op_code_for_emit_int_value(arg);
        InstructionArgument* argument;
        if (op_code == JVMOpCode::bipush)
            argument = new CConstInstructionArgument((char)arg);
        else
            argument =  new IConstInstructionArgument(arg);
        emit_op_code(op_code, argument);
    }
}

void JAsmBuilder::emit_constant(float arg) {
    if (arg == 0) 
        emit_op_code(JVMOpCode::fconst_0);
    else if (arg == 1) 
        emit_op_code(JVMOpCode::fconst_1);
    else if (arg == 2) 
        emit_op_code(JVMOpCode::fconst_2);
    else
        emit_op_code(JVMOpCode::ldc, new FConstInstructionArgument(arg));
}

void JAsmBuilder::emit_constant(std::string arg) {
    emit_op_code(JVMOpCode::ldc, new SConstInstructionArgument(arg));
}


JVMOpCode JAsmBuilder::op_code_for_emit_int_value(int value) {
    if (-128 <= value && value < 127)
        return JVMOpCode::bipush;
    if (-32768 <= value && value < 32767)
        return JVMOpCode::sipush;
    
    return JVMOpCode::ldc;
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


void JAsmBuilder::emit_global_load(Symbols::VariableSymbol* global, std::string containing_class_name) {
    auto instr_arg = new FieldAccessorInstructionArgument(containing_class_name, global);

    // Since we're putting a halt on structs, we pretty much can always just call getstatic
    auto op_code = JVMOpCode::getstatic;
    record_instruction(new Instruction(op_code, instr_arg));
}


void JAsmBuilder::emit_global_store(Symbols::VariableSymbol* global, std::string containing_class_name) {
    auto instr_arg = new FieldAccessorInstructionArgument(containing_class_name, global);

    // Since we're putting a halt on structs, we pretty much can always just call putstatic
    auto op_code = JVMOpCode::putstatic;
    record_instruction(new Instruction(op_code, instr_arg));
}



std::vector<std::string> JAsmBuilder::current_instruction_listing() const {
    std::vector<std::string> instr_list;

    for (auto instr : m_instructions) 
        instr_list.push_back(instr->str());

    return instr_list;
}

FinalizedBody* JAsmBuilder::finalize() {
    std::vector<Instruction*> instr_list;
    run_label_fixes();
    for (auto instr : m_instructions) 
        instr_list.push_back(instr);

    return new FinalizedBody(instr_list, m_locals.size(), m_builder_state->max_stack_size());
}


int JAsmBuilder::instructions_emitted() const {
    return m_builder_state->op_codes_emitted();
}

int JAsmBuilder::current_stack_size() const {
    return m_builder_state->current_stack_size();
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
        if (*(m_locals[i]) == *local) return (int)i;
    }
    return -1;
}

void JAsmBuilder::mark_branch_for_fix_up(int instruction_idx, Binding::BoundLabel* label) {
    auto fix_up_data  = new LabelFixUpData(instruction_idx, label);
    m_fixups.push_back(fix_up_data);
}

void JAsmBuilder::track_label(Binding::BoundLabel* label, int instruction_idx) {
    m_label_instr_map[label] = instruction_idx;
}

void JAsmBuilder::run_label_fixes() {

    // I use two vectors instead of a map so that I can gaurantee
    // that the order I emit the fixed labels is the same order
    // that I saw them in.
    std::vector<LabelInstructionArgument*> existing_labels;
    std::vector<int> calculated_label_pos;

    for (auto fixup : m_fixups) {
        auto target_label = fixup->label();
        auto target_instruction_idx = m_label_instr_map[target_label];
        //__attribute__((unused)) auto target_instruction = m_instructions[target_instruction_idx];
        auto inst_to_fix = m_instructions[fixup->instruction_index()];

        existing_labels.push_back(dynamic_cast<LabelInstructionArgument*>(inst_to_fix->argument()));

        calculated_label_pos.push_back(target_instruction_idx);
        //((LabelInstructionArgument*)(inst_to_fix->argument()))->bound_label()->m_name = "L" + std::to_string(counter++);
    }

    for (std::size_t idx = 0; idx < existing_labels.size(); idx ++) {
        std::string new_label = "L" + std::to_string(idx + 1);
        existing_labels[idx]->bound_label()->m_name = new_label;
        //auto instr_loc = m_instructions.begin() + calculated_label_pos[idx];
        m_instructions[calculated_label_pos[idx]]->set_label(new_label);
        //m_instructions.insert(instr_loc, new Instruction(new_label));
    }
}

void JAsmBuilder::record_instruction(Instruction* instruction) {
    m_builder_state->add_op_code(instruction->op_code());
    m_instructions.push_back(instruction);
}

void JAsmBuilder::mark_label(Binding::BoundLabel* label) {
    track_label(label, m_builder_state->op_codes_emitted());
}

}