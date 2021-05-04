#include "code_generator.h"
#include "code_gen_payload.h"

#include "../binding/bound_function_definition_node.h"
#include "../symbols/function_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/variable_symbol.h"
#include "../symbols/type_symbol.h"

namespace CodeGen {

CodeGenerator::CodeGenerator(CodeGenPayload* payload, bool debug) :
    m_payload(payload), 
    m_debug_mode(debug) {

    if (debug)
        m_outstream = &std::cout; // override any output option in the payload
    else
        m_outstream = new std::ofstream(payload->output_filename());
}

bool CodeGenerator::emit() {
    return false;
}

bool CodeGenerator::is_debug() const {
    return m_debug_mode;
}

void CodeGenerator::emit_global_variable_declaration(__attribute__((unused)) Symbols::VariableSymbol* global_variable) {

}

void CodeGenerator::emit_struct_declaration(__attribute__((unused)) Binding::BoundStructDeclarationNode* struct_decl) {

}

void CodeGenerator::emit_method(__attribute__((unused)) Binding::BoundFunctionDefinitionNode* method) {

}



}