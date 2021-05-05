#include "bound_function_definition_node.h"
#include "../symbols/function_symbol.h"
#include "bound_block_statement_node.h"
#include "bound_node_factory.h"
#include "../symbols/symbol_factory.h"

namespace Binding {

BoundFunctionDefinitionNode::BoundFunctionDefinitionNode(Symbols::FunctionSymbol* function_symbol, BoundBlockStatementNode* statements) :
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
BoundBlockStatementNode* BoundFunctionDefinitionNode::statements() const {
    return m_statements;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundFunctionDefinitionNode::kind() const {
    return BoundNodeKind::FunctionDefinition;
}

BoundFunctionDefinitionNode* Factory::function_def(Symbols::FunctionSymbol* symbol, std::vector<BoundStatementNode*> statements) {
    return function_def(symbol, block(statements));
}

BoundFunctionDefinitionNode* Factory::function_def(Symbols::FunctionSymbol* symbol, BoundBlockStatementNode* body) {
    return new BoundFunctionDefinitionNode(symbol, body);
}

BoundFunctionDefinitionNode* Factory::function_init(BoundBlockStatementNode* block) {
    return function_def(Symbols::Factory::init_function(), block);
}

BoundFunctionDefinitionNode* Factory::function_clinit(BoundBlockStatementNode* block) {
    return function_def(Symbols::Factory::clinit_function(), block);
}


}