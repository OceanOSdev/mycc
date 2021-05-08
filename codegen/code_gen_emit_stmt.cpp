#include <stdexcept>

#include "code_generator.h"
#include "../jvm_processor/j_asm_builder.h"
#include "../jvm_processor/jvm_opcodes.h"
#include "../jvm_processor/instruction_arguments.h"

#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"

#include "../binding/bound_node_kind.h"
#include "../binding/bound_node_factory.h"
#include "../binding/bound_block_statement_node.h"
#include "../binding/bound_return_statement_node.h"
#include "../binding/bound_goto_statement_node.h"
#include "../binding/bound_conditional_goto_statement_node.h"
#include "../binding/bound_label_statement_node.h"
#include "../binding/bound_variable_group_declaration_node.h"
#include "../binding/bound_expression_statement_node.h"
#include "../binding/bound_struct_declaration_node.h"
#include "../binding/bound_expression_node.h"
#include "../binding/bound_assignment_expression_node.h"
#include "../binding/bound_literal_val_expression_node.h"

#include "../binding/bound_binary_expression_node.h"
#include "../binding/bound_unary_expression_node.h"
#include "../binding/bound_binary_op_kind.h"

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
        case BoundNodeKind::ExpressionStatement:
            emit_expression(dynamic_cast<BoundExpressionStatementNode*>(statement)->expression(), false);
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
    auto label = goto_statement->label();
    auto arg = new JVMProcessor::LabelInstructionArgument(label);
    m_builder->emit_branch_op_code(JVMProcessor::JVMOpCode::_goto, arg);
}

void CodeGenerator::emit_conditional_goto_statement(Binding::BoundConditionalGotoStatementNode* cond_goto_statement) {
    auto condition = cond_goto_statement->condition();
    auto label = cond_goto_statement->label();
    emit_conditional_branch(condition, label, cond_goto_statement->jump_if_true());
}

void CodeGenerator::emit_label_statement(Binding::BoundLabelStatementNode* label_statement) {
    auto label = label_statement->label();
    m_builder->mark_label(label);
}

void CodeGenerator::emit_variable_group_declaration(Binding::BoundVariableGroupDeclarationNode* var_group_decl) {
    for (auto decl : var_group_decl->variable_declarations()) {
        emit_variable_declaration(decl);
    }
}

void CodeGenerator::emit_variable_declaration(Binding::BoundVariableDeclarationNode* variable_declaration) {
    m_builder->declare_local(variable_declaration->variable_symbol());
    if (variable_declaration->is_initialized()) {
        auto assignment = Binding::Factory::assignment(variable_declaration->variable_symbol(), variable_declaration->initializer());
        emit_expression(assignment, false);
    } else if (variable_declaration->variable_symbol()->is_array()) {
        auto type = variable_declaration->variable_symbol()->var_type();
        // we don't want to emit the '[' character present in the array type 
        // (or the const thing, if that even gets passed along)
        type = type->as_array_element_type()->as_mutable_type();
        auto arg = new JVMProcessor::TypeIdInstructionArgument(type);
        emit_expression(Binding::Factory::literal(variable_declaration->variable_symbol()->array_size()),true);
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::newarray, arg);
        emit_store(variable_declaration->variable_symbol());
    }
}


void CodeGenerator::emit_conditional_branch(Binding::BoundExpressionNode* condition, Binding::BoundLabel*& label, bool jmp_if_true) {
    bool reiterate = false;
    /// TODO: optimize for constants?

    do {
        reiterate = false;
        switch (condition->kind()) {
            case Binding::BoundNodeKind::BinaryExpression: 
            {
                emit_conditional_branch_bin_op(condition, label, jmp_if_true, reiterate);
                if (reiterate) continue;  
                break; 
            }
            case Binding::BoundNodeKind::UnaryExpression:
            {
                auto unary_op = dynamic_cast<Binding::BoundUnaryExpressionNode*>(condition);
                if (unary_op->op()->op_kind() == Binding::BoundUnaryOpKind::LogicalNegation) {
                    jmp_if_true = !jmp_if_true;
                    condition = unary_op->expression();
                    reiterate = true;
                }
                break;
            }
            default: 
            {
                emit_expression(condition, true);
                JVMProcessor::JVMOpCode op_code = jmp_if_true ? JVMProcessor::JVMOpCode::ifeq : JVMProcessor::JVMOpCode::ifne;
                if (label == nullptr) {
                    label = generate_temp_label();
                }
                auto arg = new JVMProcessor::LabelInstructionArgument(label);
                m_builder->emit_branch_op_code(op_code, arg);
                return;
            }
        }
    } while (reiterate);
}

void CodeGenerator::emit_conditional_branch_bin_op(Binding::BoundExpressionNode*& expr, Binding::BoundLabel*& destination, bool jmp_if_true, bool& should_repeat) {
    bool test_both_arguments = jmp_if_true;
    should_repeat = false;
    auto bin_expr = dynamic_cast<Binding::BoundBinaryExpressionNode*>(expr);

    switch (bin_expr->op()->op_kind()) {
        case Binding::BoundBinaryOpKind::LogicalOr:
            test_both_arguments = !test_both_arguments;
            [[fallthrough]];
        case Binding::BoundBinaryOpKind::LogicalAnd:
        {
            if (test_both_arguments) {
                Binding::BoundLabel* fall_through = nullptr;
                emit_conditional_branch(bin_expr->left(), fall_through, !jmp_if_true);
                emit_conditional_branch(bin_expr->right(), destination, jmp_if_true);
                if (fall_through != nullptr)
                    m_builder->mark_label(fall_through);
            } else {
                emit_conditional_branch(bin_expr->left(), destination, jmp_if_true);
                expr = bin_expr->right();
                should_repeat = true;
            }
            break;
        }
        case Binding::BoundBinaryOpKind::Equals: [[fallthrough]];
        case Binding::BoundBinaryOpKind::NotEquals: [[fallthrough]];
        case Binding::BoundBinaryOpKind::LessThan: [[fallthrough]];
        case Binding::BoundBinaryOpKind::LessThanOrEquals: [[fallthrough]];
        case Binding::BoundBinaryOpKind::GreaterThan: [[fallthrough]];
        case Binding::BoundBinaryOpKind::GreaterThanOrEquals:
        {
            emit_expression(bin_expr->left(), true);
            emit_expression(bin_expr->right(), true);
            JVMProcessor::JVMOpCode reverse_op_code;
            JVMProcessor::JVMOpCode op_code = get_jump_code(bin_expr->op(), jmp_if_true, reverse_op_code);
            if (destination == nullptr) {
                destination = generate_temp_label();
            }

            auto arg = new JVMProcessor::LabelInstructionArgument(destination);
            m_builder->emit_branch_op_code(op_code, arg);
            break;
        }
        default: break;
    }
}

using j_op_code = JVMProcessor::JVMOpCode;
const int jvm_op_code_row_length = 4;
const int order_cmp_offset = 0;
const int eq_cmp_offset = 16;
const int invert_idx_offset = 4; // could use jvm_op_code_row_length, but this name makes it more clear as to what I'm doing with it.

const j_op_code conditional_jump_op_codes[] = {
    //  <                 //  <=                //  >                 //  >=
    j_op_code::if_icmplt, j_op_code::if_icmple, j_op_code::if_icmpgt, j_op_code::if_icmpge, // regular
    j_op_code::if_icmpge, j_op_code::if_icmpgt, j_op_code::if_icmple, j_op_code::if_icmplt, // reverse
    j_op_code::iflt,      j_op_code::ifle,      j_op_code::ifgt,      j_op_code::ifge,      // regular (float)
    j_op_code::ifge,      j_op_code::ifgt,      j_op_code::ifle,      j_op_code::iflt,      // reverse (float)

    //  ==                //  !=                // two repetitions to pad the row so that the rev offset can remain consistent
    j_op_code::if_icmpeq, j_op_code::if_icmpne, j_op_code::if_icmpeq, j_op_code::if_icmpne, // regular
    j_op_code::if_icmpne, j_op_code::if_icmpeq, j_op_code::if_icmpne, j_op_code::if_icmpeq, // reverse
    j_op_code::ifeq,      j_op_code::ifne,      j_op_code::ifeq,      j_op_code::ifne,      // regular (float)
    j_op_code::ifne,      j_op_code::ifeq,      j_op_code::ifne,      j_op_code::ifeq,      // regular (float)
};

JVMProcessor::JVMOpCode CodeGenerator::get_jump_code(Binding::BoundBinaryOperatorNode* bin_op, bool jmp_if_true, JVMProcessor::JVMOpCode& reverse_op_code) {
    int op_idx;
    switch (bin_op->op_kind()) {
        case Binding::BoundBinaryOpKind::Equals: op_idx = eq_cmp_offset; break;
        case Binding::BoundBinaryOpKind::NotEquals: op_idx = eq_cmp_offset + 1; break;
        case Binding::BoundBinaryOpKind::LessThan: op_idx = order_cmp_offset; break;
        case Binding::BoundBinaryOpKind::LessThanOrEquals: op_idx = order_cmp_offset + 1; break;
        case Binding::BoundBinaryOpKind::GreaterThan: op_idx = order_cmp_offset + 2; break;
        case Binding::BoundBinaryOpKind::GreaterThanOrEquals:op_idx = order_cmp_offset + 3; break;
        default: throw new std::runtime_error("Invalid jump code");
    }

    using namespace Symbols;
    if (TypeSymbol::are_types_equal(bin_op->type(), &TypeSymbol::Float)) {
        op_idx += 4 * jvm_op_code_row_length; // the equivalent comparison op for floats is 16 elements ahead of the original offset.
    }

    int reverse_op_idx = op_idx;
    if (!jmp_if_true) op_idx += invert_idx_offset;
    else reverse_op_idx += invert_idx_offset;

    reverse_op_code = conditional_jump_op_codes[reverse_op_idx];
    return conditional_jump_op_codes[op_idx];
}
}