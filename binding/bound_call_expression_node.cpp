#include "bound_call_expression_node.h"
#include "../symbols/type_symbol.h"
#include "../symbols/function_symbol.h"
#include "bound_node_factory.h"

namespace Binding {

BoundCallExpressionNode* Factory::call(Symbols::FunctionSymbol* function, std::vector<BoundExpressionNode*> arguments) {
    return new BoundCallExpressionNode(function, arguments);
}

BoundCallExpressionNode::BoundCallExpressionNode(Symbols::FunctionSymbol* function, std::vector<BoundExpressionNode*> arguments) :
    m_function(function), m_arguments(arguments) {}


Symbols::FunctionSymbol* BoundCallExpressionNode::function() const {
    return m_function;
}

std::vector<BoundExpressionNode*> BoundCallExpressionNode::arguments() const {
    return m_arguments;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundCallExpressionNode::type() const {
    return m_function->type();
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundCallExpressionNode::kind() const {
    return BoundNodeKind::CallExpression;
}

}