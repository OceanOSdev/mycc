#pragma once
#ifndef BOUND_BLOCK_STATEMENT_NODE_H
#define BOUND_BLOCK_STATEMENT_NODE_H

#include "bound_statement_node.h"
#include <vector>

namespace Binding {

class BoundBlockStatementNode : public BoundStatementNode {
private:
    std::vector<BoundStatementNode*> m_statements;
public:
    BoundBlockStatementNode(std::vector<BoundStatementNode*> statements);

    /*
     * The list of statements encapsulated by the block.
     */
    std::vector<BoundStatementNode*> statements() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;
};

}

#endif