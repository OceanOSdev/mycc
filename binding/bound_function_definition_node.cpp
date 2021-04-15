#include "bound_function_definition_node.h"
#include "../symbols/function_symbol.h"
#include "bound_block_statement_node.h"

namespace Binding {

BoundFunctionDefinitionNode::BoundFunctionDefinitionNode(Symbols::FunctionSymbol* function_symbol, BoundBlockStatement* statements) :
    m_function_symbol(function_symbol), m_statements(statements) {}

/*
 * Symbol representing this bound function.
 */
Symbols::FunctionSymbol* BoundFunctionDefinitionNode::function_symbol() const {
    return m_function_symbol;
}

/*
 * The statements in the body of the function.
 */
BoundBlockStatement* BoundFunctionDefinitionNode::statements() const {
    return m_statements;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundFunctionDefinitionNode::kind() const {
    return BoundNodeKind::FunctionDefinition;
}

}