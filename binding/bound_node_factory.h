#pragma once
#ifndef BOUND_NODE_FACTORY_H
#define BOUND_NODE_FACTORY_H

#include <string>
#include <vector>

namespace Symbols {
class FunctionSymbol;
class VariableSymbol;
} // namespace Symbols

namespace Binding {
class BoundBlockStatementNode;
class BoundStatementNode;
class BoundExpressionStatementNode;
class BoundCallExpressionNode;
class BoundExpressionNode;
class BoundLiteralValExpressionNode;
class BoundAssignmentExpressionNode;
class BoundVariableReferenceExpressionNode;
class BoundFunctionDefinitionNode;

namespace Factory {

BoundBlockStatementNode *block(std::vector<BoundStatementNode *> statements);
BoundExpressionStatementNode *statement(BoundExpressionNode *expression);
BoundCallExpressionNode *call(Symbols::FunctionSymbol *function,
                              std::vector<BoundExpressionNode *> arguments);
BoundAssignmentExpressionNode *assignment(Symbols::VariableSymbol *variable,
                                          BoundExpressionNode *expression);
BoundVariableReferenceExpressionNode *
var_ref(Symbols::VariableSymbol *variable);
BoundLiteralValExpressionNode *literal(char literal);
BoundLiteralValExpressionNode *literal(int literal);
BoundLiteralValExpressionNode *literal(float literal);
BoundLiteralValExpressionNode *literal(std::string literal);

BoundFunctionDefinitionNode *
function_def(Symbols::FunctionSymbol *symbol,
             std::vector<BoundStatementNode *> statements);
BoundFunctionDefinitionNode *function_def(Symbols::FunctionSymbol *symbol,
                                          BoundBlockStatementNode *body);
BoundFunctionDefinitionNode *function_init(BoundBlockStatementNode *block);
BoundFunctionDefinitionNode *function_clinit(BoundBlockStatementNode *block);

} // namespace Factory
} // namespace Binding

#endif