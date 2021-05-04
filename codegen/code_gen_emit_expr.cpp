#include <stdexcept>

#include "code_generator.h"

#include "../binding/bound_expression_node.h"
#include "../binding/bound_node_kind.h"

namespace CodeGen {

void CodeGenerator::emit_expression(Binding::BoundExpressionNode* expression, __attribute__((unused)) bool used) {
    using namespace Binding;
    auto kind = expression->kind();
    switch (kind) {
        default:
            throw new std::runtime_error("Invalid expression type while emitting: " + std::to_string(kind));
    }
}


}