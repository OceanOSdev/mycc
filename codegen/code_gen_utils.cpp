#include "code_generator.h"
#include "../jvm_processor/j_asm_builder.h"
#include "../jvm_processor/jvm_opcodes.h"

#include "../symbols/function_symbol.h"
#include "../symbols/type_symbol.h"

#include "../binding/bound_label_statement_node.h"
#include <string>
#include <cstdlib>

namespace CodeGen {


void CodeGenerator::emit_pop_if_unused(bool used) {
    if (!used)
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::pop);
}

void CodeGenerator::declare_local(Symbols::VariableSymbol* local_variable) {
    m_builder->declare_local(local_variable);
}

Binding::BoundLabel* CodeGenerator::generate_temp_label() const {
    // try to create a unique name 
    // (only has to be unique to this method since the lifetime of our builder 
    // is the duration of our emitting of this method)
    std::string cur_stack_str = std::to_string(m_builder->current_stack_size());
    std::string op_codes_emitted_str = std::to_string(m_builder->instructions_emitted());
    std::string rand_padding = std::to_string(std::rand()); // just for good measure.
    return new Binding::BoundLabel(cur_stack_str + "_" + op_codes_emitted_str + "__" + rand_padding);
}

int CodeGenerator::calculate_call_stack_delta(Symbols::FunctionSymbol* method) {
    int stack_delta = 0;
    // check whether the method is a void type or not
    if (!Symbols::TypeSymbol::is_error_or_incomplete_type(method->type())) {
        // if it isn't, calling this method puts the return value on the stack.
        stack_delta++;
    }

    // and of course, calling the method pops all of the arguments off the stack.
    stack_delta -= method->params().size();
    return stack_delta;
}

}