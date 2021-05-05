#include <stdexcept>

#include "code_generator.h"

#include "../jvm_processor/j_asm_builder.h"

#include "../symbols/type_symbol.h"

#include "../binding/bound_expression_node.h"
#include "../binding/bound_literal_val_expression_node.h"
#include "../binding/bound_node_kind.h"

namespace CodeGen {

void CodeGenerator::emit_expression(Binding::BoundExpressionNode* expression, bool used) {
    if (expression == nullptr) return; // I don't believe we should ever hit this case, but there might be an edge case I missed.

    // Let's get the case that we're dealing with a constant expression out of the way.
    if (expression->kind() == Binding::BoundNodeKind::LiteralExpression) {
        if (!used) return; // If we're not using it there's no reason to pollute the stack with it.
        auto lit = dynamic_cast<Binding::BoundLiteralValExpressionNode*>(expression);
        auto type = lit->type();
        if (Symbols::TypeSymbol::are_types_equal(&Symbols::TypeSymbol::Char, type))
            m_builder->emit_constant((int)lit->char_value());
        else if (Symbols::TypeSymbol::are_types_equal(&Symbols::TypeSymbol::Int, type))
            m_builder->emit_constant(lit->int_value());
        else if (Symbols::TypeSymbol::are_types_equal(&Symbols::TypeSymbol::Float, type))
            m_builder->emit_constant(lit->float_value());
        else // The only other literal type is string
            m_builder->emit_constant(lit->string_value());
        
        return;        
    }

    emit_expression_driver(expression, used);
}

void CodeGenerator::emit_expression_driver(Binding::BoundExpressionNode* expression, __attribute__((unused)) bool used) {
    using namespace Binding;
    auto kind = expression->kind();
    switch (kind) {
        case BoundNodeKind::AssignmentExpression: break;
        case BoundNodeKind::CallExpression: break;
        default:
            throw new std::runtime_error("Invalid expression type while emitting: " + std::to_string(kind));
    }
}


}