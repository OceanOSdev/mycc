#include "code_generator.h"
#include "../jvm_processor/j_asm_builder.h"
#include "../jvm_processor/jvm_opcodes.h"

#include "../symbols/function_symbol.h"
#include "../symbols/type_symbol.h"

namespace CodeGen {

void CodeGenerator::emit_pop_if_unused(bool used) {
    if (!used)
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::pop);
}

void CodeGenerator::declare_local(Symbols::VariableSymbol* local_variable) {
    m_builder->declare_local(local_variable);
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