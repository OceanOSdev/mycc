#pragma once
#ifndef BOUND_FUNCTION_DEFINITION_NODE_H
#define BOUND_FUNCTION_DEFINITION_NODE_H

#include "bound_global_declaration_node.h"

namespace Symbols {
    class FunctionSymbol;
}

namespace Binding {
class BoundBlockStatement;

class BoundFunctionDefinitionNode : public BoundGlobalDeclarationNode {
private:
    Symbols::FunctionSymbol* m_function_symbol;
    BoundBlockStatement* m_statements;
public:
    BoundFunctionDefinitionNode(Symbols::FunctionSymbol* function_symbol, BoundBlockStatement* statements);

    /*
     * Symbol representing this bound function.
     */
    Symbols::FunctionSymbol* function_symbol() const;

    /*
     * The statements in the body of the function.
     */
    BoundBlockStatement* statements() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

};

}

#endif