#include <stdexcept>

#include "code_generator.h"
#include "code_gen_payload.h"
#include "method_reference.h"

#include "../jvm_processor/j_asm_builder.h"
#include "../jvm_processor/jvm_opcodes.h"
#include "../jvm_processor/instruction_arguments.h"

#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"

#include "../binding/bound_expression_node.h"
#include "../binding/bound_literal_val_expression_node.h"
#include "../binding/bound_assignment_expression_node.h"
#include "../binding/bound_call_expression_node.h"
#include "../binding/bound_cast_expression_node.h"
#include "../binding/bound_variable_reference_expression_node.h"
#include "../binding/bound_increment_expression_node.h"
#include "../binding/bound_binary_expression_node.h"
#include "../binding/bound_unary_expression_node.h"
#include "../binding/bound_index_expression_node.h"
#include "../binding/bound_node_kind.h"

namespace CodeGen {

void CodeGenerator::emit_expression(Binding::BoundExpressionNode* expression, bool used) {
    if (expression == nullptr) return; // I don't believe we should ever hit this case, but there might be an edge case I missed.

    // Let's get the case that we're dealing with a constant expression out of the way.
    if (expression->kind() == Binding::BoundNodeKind::LiteralExpression) {
        if (!used) return; // If we're not using it there's no reason to pollute the stack with it.
        auto lit = dynamic_cast<Binding::BoundLiteralValExpressionNode*>(expression);
        auto type = lit->type();
        if (Symbols::TypeSymbol::are_types_equal(&Symbols::TypeSymbol::Char, type)) {
            char c_val = lit->char_value();
            m_builder->emit_constant((int)c_val);
        } else if (Symbols::TypeSymbol::are_types_equal(&Symbols::TypeSymbol::Int, type))
            m_builder->emit_constant(lit->int_value());
        else if (Symbols::TypeSymbol::are_types_equal(&Symbols::TypeSymbol::Float, type))
            m_builder->emit_constant(lit->float_value());
        else { 
            // The only other literal type is string
            // however we also have to implicitly "cast"
            // this to a char array.
            m_builder->emit_constant(lit->string_value());
            auto op_code = JVMProcessor::JVMOpCode::invokevirtual;
            auto owner = _to_char_array_method_reference->owner();
            auto symbol = _to_char_array_method_reference->method();
            auto args = new JVMProcessor::MethodCallInstructionArgument(owner, symbol);
            int stack_delta = calculate_call_stack_delta(symbol);
            m_builder->emit_op_code(op_code, args, stack_delta);

        }
        
        return;        
    }

    emit_expression_driver(expression, used);
}

void CodeGenerator::emit_expression_driver(Binding::BoundExpressionNode* expression, bool used) {
    using namespace Binding;
    auto kind = expression->kind();
    switch (kind) {
        case BoundNodeKind::AssignmentExpression:
            emit_assignment_expression(dynamic_cast<BoundAssignmentExpressionNode*>(expression), used);
            break;
        case BoundNodeKind::CallExpression: 
            emit_call_expression(dynamic_cast<BoundCallExpressionNode*>(expression), used);
            break;
        case BoundNodeKind::CastExpression:
            emit_cast_expression(dynamic_cast<BoundCastExpressionNode*>(expression), used);
            break;
        case BoundNodeKind::VariableReferenceExpression:
            emit_load(dynamic_cast<BoundVariableReferenceExpressionNode*>(expression));
            break;
        case BoundNodeKind::IncrementExpression:
            emit_increment_expression(dynamic_cast<BoundIncrementExpressionNode*>(expression), used);
            break;
        case BoundNodeKind::DecrementExpression:
            emit_decrement_expression(dynamic_cast<BoundDecrementExpressionNode*>(expression), used);
            break;
        case BoundNodeKind::BinaryExpression: break;
        case BoundNodeKind::UnaryExpression: break;
        case BoundNodeKind::IndexExpression: 
            emit_index_expression(dynamic_cast<Binding::BoundIndexExpressionNode*>(expression), used);
            break;
        default:
            throw new std::runtime_error("Invalid expression type while emitting: " + std::to_string(kind));
    }
}


void CodeGenerator::emit_assignment_expression(Binding::BoundAssignmentExpressionNode* assignment, bool used) {
    auto is_arr = assignment->variable_reference()->kind() == Binding::BoundNodeKind::IndexExpression;
    if (!is_arr) {
        emit_expression(assignment->expression(), true);
        // in the event we have an expression like 'int a = (b = 4)' we need to duplicate (b=4) on the stack
        // so that later code gen has access to it.
        if (used) m_builder->emit_op_code(JVMProcessor::JVMOpCode::dup);
        emit_store(assignment->variable_reference());
    } else {
        // kind of hacky, but we don't want the xaload instruction emitted even if we are using
        // the value of the assignment since we just emit dup_x2 instead which should handle
        // keeping the value where it needs to be on the stack.
        emit_expression(assignment->variable_reference(), false); 
        emit_expression(assignment->expression(), true);
        // using dup_x2 instead of dup since calling xastore pops the top three
        // values off of the stack, so we want to "duplicate" the top value on
        // the stack, but insert it below the top three values.
        if (used) m_builder->emit_op_code(JVMProcessor::JVMOpCode::dup_x2);
        m_builder->emit_array_store(assignment->variable_reference()->variable_reference());

        
    }
}

void CodeGenerator::emit_call_expression(Binding::BoundCallExpressionNode* call_expression, bool used) {
    for (auto argument : call_expression->arguments()) {
        emit_expression(argument, true);
    }
    auto op_code = JVMProcessor::JVMOpCode::invokestatic;
    auto method = look_up_method(call_expression->function());
    auto args = new JVMProcessor::MethodCallInstructionArgument(method->owner(), method->method());
    int stack_delta = calculate_call_stack_delta(call_expression->function());
    m_builder->emit_op_code(op_code, args, stack_delta);

    // check to see if we need to clean up the stack if a nonvoid method's return is not used.
    if (!Symbols::TypeSymbol::is_error_or_incomplete_type(call_expression->type())) {
        emit_pop_if_unused(used);
    }
}

void CodeGenerator::emit_cast_expression(Binding::BoundCastExpressionNode* cast_expression, bool used) {
    auto castee = cast_expression->expression();
    emit_expression(castee, used);

    auto type = cast_expression->type();

    // because of the way the op codes are, we technically don't
    // need to even check the type we are casting from since we're only
    // dealing with i2f, f2i, i2b (not even really), and i2c which have all unique targets
    // unless of course I fucked up somewhere...

    using namespace Symbols;
    if (TypeSymbol::are_types_equal(type, &TypeSymbol::Char)) {
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::i2c);
    } else if (TypeSymbol::are_types_equal(type, &TypeSymbol::Int)) {
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::f2i);
    } else if (TypeSymbol::are_types_equal(type, &TypeSymbol::Float)) {
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::i2f);
    } else {
        // m_builder->emit_op_code(JVMProcessor::JVMOpCode::nop);
        // ^ actually, let's throw an exception instead.
        throw new std::runtime_error("Invalid cast during bytecode emitting.");
    }
}

void CodeGenerator::emit_load(Binding::BoundVariableReferenceExpressionNode* variable_reference_expression) {
    emit_load(variable_reference_expression->variable_reference());
}

void CodeGenerator::emit_load(Symbols::VariableSymbol* variable_reference) {
    /// TODO: if variable is constant and its val is a LVE then just push the value to the stack
    /// instead of emitting a load instruction.
    bool is_field = variable_reference->is_global();
    if (is_field)
        m_builder->emit_global_load(variable_reference, m_payload->filename_base());
    else
        m_builder->emit_local_load(variable_reference);
}

void CodeGenerator::emit_store(Binding::BoundVariableReferenceExpressionNode* variable_reference_expression) {
    emit_store(variable_reference_expression->variable_reference());
}

void CodeGenerator::emit_store(Symbols::VariableSymbol* variable_reference) {
    /// TODO: Is checking whether ref is constant or not necessary?
    /// Off of the top of my head I can't see why it would be but
    /// as always, there might be some edge case where such, and invalid
    /// state sneaks in.

    bool is_field = variable_reference->is_global();
    if (is_field)
        m_builder->emit_global_store(variable_reference, m_payload->filename_base());
    else
        m_builder->emit_local_store(variable_reference);
}

void CodeGenerator::emit_increment_expression(Binding::BoundIncrementExpressionNode* increment_expression, bool used) {
    auto ref_kind = increment_expression->expression()->kind();
    if (ref_kind != Binding::BoundNodeKind::VariableReferenceExpression && ref_kind != Binding::BoundNodeKind::IndexExpression) {
        throw new std::runtime_error("Cannot increment: not an lvalue");
    }
    emit_incdec_expression(dynamic_cast<Binding::BoundVariableReferenceExpressionNode*>(increment_expression->expression()), increment_expression->notation(), 1, used); 
}

void CodeGenerator::emit_decrement_expression(Binding::BoundDecrementExpressionNode* decrement_expression, bool used) {
    auto ref_kind = decrement_expression->expression()->kind();
    if (ref_kind != Binding::BoundNodeKind::VariableReferenceExpression && ref_kind != Binding::BoundNodeKind::IndexExpression) {
        throw new std::runtime_error("Cannot decrement: not an lvalue");
    }
    emit_incdec_expression(dynamic_cast<Binding::BoundVariableReferenceExpressionNode*>(decrement_expression->expression()), decrement_expression->notation(), -1, used);
}

void CodeGenerator::emit_incdec_expression(Binding::BoundVariableReferenceExpressionNode* ref_expression, Binding::IDNotation notation, int mutation, bool used) {
    /*
     * (WLOG we refer to increment)
     * IDEA: if used, check if pre or post increment.
     * If pre increment (i.e. ++var), then emit load after inc
     * If post increment (i.e. var++), then emit load before inc
     * 
     * If not used, don't emit load at all, only inc
     */

    using namespace Symbols;
    bool pre = used && (notation == Binding::IDNotation::PREFIX);
    bool post = used && (notation == Binding::IDNotation::POSTFIX);

    if (ref_expression->kind() != Binding::BoundNodeKind::IndexExpression) {
        // we are not dealing with an array
        auto type = ref_expression->variable_reference()->var_type()->as_mutable_type(); // <-- probably dont need as mutable since that should be caught in binder
        // Int
        if (TypeSymbol::are_types_equal(type, &TypeSymbol::Int)) {
            if (post) emit_load(ref_expression); 
            int ref_idx = m_builder->get_local_variable_index(ref_expression->variable_reference());
            auto arg = new JVMProcessor::ITupleConstInstructionArgument(ref_idx, mutation);
            m_builder->emit_op_code(JVMProcessor::JVMOpCode::iinc, arg);
            if (pre) emit_load(ref_expression);
            return; // I don't want to write a bunch of elses, so just return immediately when we're done.
        }
        // Char
        if (TypeSymbol::are_types_equal(type, &TypeSymbol::Char)) {
            if (post) emit_load(ref_expression);
            emit_load(ref_expression);
            m_builder->emit_op_code(JVMProcessor::JVMOpCode::iconst_1);
            auto op_code = mutation == 1 ? JVMProcessor::JVMOpCode::iadd : JVMProcessor::JVMOpCode::isub;
            m_builder->emit_op_code(op_code);
            m_builder->emit_op_code(JVMProcessor::JVMOpCode::i2c);
            emit_store(ref_expression);
            if (pre) emit_load(ref_expression); /// TODO: Maybe we can refactor Char so that the incdec code can be merged with FLoat?
            return;
        }
        // Float
        if (TypeSymbol::are_types_equal(type, &TypeSymbol::Float)) {
            emit_load(ref_expression);
            if (post) m_builder->emit_op_code(JVMProcessor::JVMOpCode::dup);
            m_builder->emit_op_code(JVMProcessor::JVMOpCode::fconst_1);
            auto op_code = mutation == 1 ? JVMProcessor::JVMOpCode::fadd : JVMProcessor::JVMOpCode::fsub;
            m_builder->emit_op_code(op_code);
            if (pre) m_builder->emit_op_code(JVMProcessor::JVMOpCode::dup);
            emit_store(ref_expression);
            return;
        }
    }

}

void CodeGenerator::emit_binary_expression(__attribute__((unused)) Binding::BoundBinaryExpressionNode* binary_expression, __attribute__((unused)) bool used) {

}

void CodeGenerator::emit_unary_expression(Binding::BoundUnaryExpressionNode* unary_expression, bool used) {
    auto op_kind = unary_expression->op()->op_kind();
    if (!used) {
        emit_expression(unary_expression->expression(), false);
        return;
    }

    if (op_kind == Binding::BoundUnaryOpKind::LogicalNegation) {
        emit_conditional_expression(unary_expression->expression(), false);
        return;
    }
}

void CodeGenerator::emit_index_expression(Binding::BoundIndexExpressionNode* index_expression, bool used) {
    emit_load(index_expression->variable_reference());
    emit_expression(index_expression->expression(), true);
    if (used) {
        m_builder->emit_array_load(index_expression->variable_reference());
    }
}


void CodeGenerator::emit_binary_conditional_expression(__attribute__((unused)) Binding::BoundBinaryExpressionNode* binary_expression, __attribute__((unused)) bool jmp_if_true) {

}

void CodeGenerator::emit_conditional_expression(Binding::BoundExpressionNode* condition, bool jmp_if_true) {
    while (condition->kind() == Binding::BoundNodeKind::UnaryExpression) {
        auto unary = dynamic_cast<Binding::BoundUnaryExpressionNode*>(condition);
        /// TODO: make sure that it is a logical negation
        condition = unary->expression();
        jmp_if_true = !jmp_if_true;
    }

    /// TODO: optimize for constant values

    if (condition->kind() == Binding::BoundNodeKind::BinaryExpression) {
        auto binary = dynamic_cast<Binding::BoundBinaryExpressionNode*>(condition);
        if (Binding::is_conditional_binary_operator(binary->op()->op_kind())) {
            emit_binary_conditional_expression(binary, jmp_if_true);
            return;
        }
    }

    emit_expression(condition, true);
    if (!jmp_if_true) {
        m_builder->emit_op_code(JVMProcessor::JVMOpCode::ifeq);
    }
}


}