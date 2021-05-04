#include "code_generator.h"
#include "code_gen_payload.h"

#include "../jvm_processor/j_asm_builder.h"
#include "../jvm_processor/j_asm_builder.h"

#include "../binding/bound_function_definition_node.h"
#include "../binding/bound_global_declaration_node.h"
#include "../binding/bound_global_statement_node.h"
#include "../binding/bound_variable_group_declaration_node.h"
#include "../binding/bound_statement_node.h"

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
    emit_basic_assembly_metadata(m_payload->filename_base());
    for (auto decl : m_payload->lowered_global_declarations()) {
        if (decl->kind() == Binding::BoundNodeKind::GlobalStatement) {
            auto gdec = dynamic_cast<Binding::BoundGlobalStatementNode*>(decl);
            if (gdec->statement()->kind() == Binding::BoundNodeKind::VariableGroupDeclaration) {
                auto group_decl = dynamic_cast<Binding::BoundVariableGroupDeclarationNode*>(gdec->statement());
                for (auto var_decl : group_decl->variable_declarations())
                    emit_field(var_decl, true);
            } else {
                // Emit a struct
            }
        } else {

        }
    }

    (*m_outstream) << m_builder->emit_directive_end(JVMProcessor::BlockDirectiveType::ClassDirective) << std::endl;
    return false;
}

bool CodeGenerator::is_debug() const {
    return m_debug_mode;
}

void CodeGenerator::emit_struct_declaration(__attribute__((unused)) Binding::BoundStructDeclarationNode* struct_decl) {

}

void CodeGenerator::emit_method(__attribute__((unused)) Binding::BoundFunctionDefinitionNode* method) {

}

void CodeGenerator::emit_field(Binding::BoundVariableDeclarationNode* field, bool is_static) {
    bool needs_clinit;
    std::string output = JVMProcessor::JAsmBuilder::emit_field_directive(field, is_static, needs_clinit);
    if (needs_clinit)
        m_fields_needing_clinit.push_back(field);
    
    (*m_outstream) << output << std::endl;
}

void CodeGenerator::emit_basic_assembly_metadata(std::string class_name) {
    (*m_outstream) << JVMProcessor::JAsmBuilder::emit_class_directive(class_name) << std::endl;
    (*m_outstream) << JVMProcessor::JAsmBuilder::emit_super_directive() << std::endl;
}

}