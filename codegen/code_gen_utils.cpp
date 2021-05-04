#include "code_generator.h"
#include "../jvm_processor/j_asm_builder.h"
#include "../jvm_processor/jvm_opcodes.h"

namespace CodeGen {

void CodeGenerator::emit_pop_if_unused(bool used) {
    if (!used)
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::pop);
}

void CodeGenerator::declare_local(Symbols::VariableSymbol* local_variable) {
    m_builder->declare_local(local_variable);
}

}