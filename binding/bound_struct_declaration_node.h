#pragma once
#ifndef BOUND_STRUCT_DECLARATION_NODE_H
#define BOUND_STRUCT_DECLARATION_NODE_H

#include "bound_statement_node.h"

namespace Symbols {
    class StructSymbol;
}

namespace Binding {

class BoundStructDeclarationNode : public BoundStatementNode {
private:
    Symbols::StructSymbol* m_struct_symbol;
public:
    BoundStructDeclarationNode(Symbols::StructSymbol* struct_symbol);

    Symbols::StructSymbol* struct_symbol() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;
};

}

#endif