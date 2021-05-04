#include "builder_state.h"
#include "jvm_opcodes.h"

namespace JVMProcessor {

BuilderState::BuilderState() :
    m_max_stack_size(0),
    m_current_stack_size(0),
    m_op_codes_emitted(0) {}

int BuilderState::max_stack_size() const {
    return m_max_stack_size;
}

int BuilderState::current_stack_size() const {
    return m_current_stack_size;
}

int BuilderState::op_codes_emitted() const {
    return m_op_codes_emitted;
}

void BuilderState::add_op_code(__attribute__((unused)) JVMOpCode op_code) {
    m_op_codes_emitted++;
}

std::stack<StackMachineItem> BuilderState::current_stack_contents() const {
    return m_current_stack_contents;
}

void BuilderState::adjust_stack(int net_change) {
    m_current_stack_size += net_change;
    m_max_stack_size = m_max_stack_size < m_current_stack_size 
        ? m_current_stack_size : m_max_stack_size;

    ///TODO: Create/Expose/Implement method to record
    /// modifications to m_current_stack_contents. 
}



}