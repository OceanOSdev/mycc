#pragma once
#ifndef BOUND_EMPTY_STATEMENT_NODE_H
#define BOUND_EMPTY_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {

class BoundEmptyStatementNode : public BoundStatementNode {
private:
public:
    BoundEmptyStatementNode();

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

};

}

#endif