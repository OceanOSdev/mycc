#ifndef SYNTAX_BLOCK_STATEMENT_NODE_H
#define SYNTAX_BLOCK_STATEMENT_NODE_H

#include <vector>
#include "statement_node.h"

namespace Syntax {

class BlockStatementNode : public StatementNode {
private:
    std::vector<StatementNode*> stmt_nodes;
public:
    BlockStatementNode(std::vector<StatementNode*> stmts) : stmt_nodes(stmts) { }
    ~BlockStatementNode() { 
        std::vector<StatementNode*>::iterator stmt_iter;
        for (stmt_iter = stmt_nodes.begin(); stmt_iter != stmt_nodes.end(); stmt_iter++)
            delete *stmt_iter;
    }

    /*
     * The list of statements in this code block.
     */
    std::vector<StatementNode*> statements() const { return stmt_nodes; }
};

}

#endif