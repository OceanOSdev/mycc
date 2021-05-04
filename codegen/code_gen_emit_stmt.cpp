#include <stdexcept>

#include "code_generator.h"
#include "../jvm_processor/j_asm_builder.h"
#include "../jvm_processor/jvm_opcodes.h"
#include "../jvm_processor/instruction_arguments.h"

#include "../symbols/type_symbol.h"

#include "../binding/bound_node_kind.h"
#include "../binding/bound_block_statement_node.h"
#include "../binding/bound_return_statement_node.h"
#include "../binding/bound_goto_statement_node.h"
#include "../binding/bound_conditional_goto_statement_node.h"
#include "../binding/bound_label_statement_node.h"
#include "../binding/bound_variable_group_declaration_node.h"
#include "../binding/bound_expression_statement_node.h"
#include "../binding/bound_struct_declaration_node.h"
#include "../binding/bound_expression_node.h"

namespace CodeGen {

void CodeGenerator::emit_statement(Binding::BoundStatementNode* statement) {
    auto kind = statement->kind();
    using namespace Binding;
    switch (kind) {
        case BoundNodeKind::BlockStatement:
            emit_block_statement(dynamic_cast<BoundBlockStatementNode*>(statement));
            break;
        case BoundNodeKind::ReturnStatement:
            emit_return_statement(dynamic_cast<BoundReturnStatementNode*>(statement));
            break;
        case BoundNodeKind::GotoStatement:
            emit_goto_statement(dynamic_cast<BoundGotoStatementNode*>(statement));
            break;
        case BoundNodeKind::ConditionalGotoStatement:
            emit_conditional_goto_statement(dynamic_cast<BoundConditionalGotoStatementNode*>(statement));
            break;
        case BoundNodeKind::LabelStatement:
            emit_label_statement(dynamic_cast<BoundLabelStatementNode*>(statement));
            break;
        case BoundNodeKind::VariableGroupDeclaration:
            emit_variable_group_declaration(dynamic_cast<BoundVariableGroupDeclarationNode*>(statement));
            break;
        case BoundNodeKind::VariableDeclaration: // <-- gets handled above but I figured I'd add it in anyways just in case I miss some edge case where we recurse and hit this unexpectedly.
            emit_variable_declaration(dynamic_cast<BoundVariableDeclarationNode*>(statement));
            break;
        case BoundNodeKind::StructDeclaration:
            emit_struct_declaration(dynamic_cast<BoundStructDeclarationNode*>(statement));
            break;
        default:
            throw new std::runtime_error("Invalid statement type while emitting: " + std::to_string(kind));
    }
}

void CodeGenerator::emit_block_statement(Binding::BoundBlockStatementNode* block_statement) {
    for (auto statement : block_statement->statements()) {
        emit_statement(statement);
    }
}

void CodeGenerator::emit_return_statement(Binding::BoundReturnStatementNode* return_statement) {
    if (return_statement->expression() != nullptr) {
        emit_expression(return_statement->expression(), true);
        auto type = return_statement->expression()->type();
        if (Symbols::TypeSymbol::is_bytecode_reference_type(type)) {
            m_builder->emit_return_op_code(JVMProcessor::ReturnOpType::Object);
        } else {
            bool is_int_ret = Symbols::TypeSymbol::are_types_equivalent(type, &Symbols::TypeSymbol::Int);
            m_builder->emit_return_op_code(is_int_ret ? 
                JVMProcessor::ReturnOpType::Integer : JVMProcessor::ReturnOpType::Float);
        }
    } else {
        m_builder->emit_return_op_code();
    }
}

void CodeGenerator::emit_goto_statement(Binding::BoundGotoStatementNode* goto_statement) {
    auto label = goto_statement->label()->name();
    auto arg = new JVMProcessor::LabelInstructionArgument(label);
    m_builder->emit_branch_op_code(JVMProcessor::JVMOpCode::_goto, arg);
}

void CodeGenerator::emit_conditional_goto_statement(__attribute__((unused)) Binding::BoundConditionalGotoStatementNode* cond_goto_statement) {

}

void CodeGenerator::emit_label_statement(__attribute__((unused)) Binding::BoundLabelStatementNode* label_statement) {

}

void CodeGenerator::emit_variable_group_declaration(Binding::BoundVariableGroupDeclarationNode* var_group_decl) {
    for (auto decl : var_group_decl->variable_declarations()) {
        emit_variable_declaration(decl);
    }
}

void CodeGenerator::emit_variable_declaration(Binding::BoundVariableDeclarationNode* variable_declaration) {
    m_builder->declare_local(variable_declaration->variable_symbol());
    if (variable_declaration->is_initialized())
        emit_expression(variable_declaration->initializer(), true);
}

}