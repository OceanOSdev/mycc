#pragma once
#ifndef SYNTAX_BLOCK_STATEMENT_NODE_H
#define SYNTAX_BLOCK_STATEMENT_NODE_H

#include <vector>
#include "statement_node.h"

namespace Syntax {
class SyntaxToken;
class BlockStatementNode : public StatementNode {
private:
    std::vector<StatementNode*> stmt_nodes;
public:
    BlockStatementNode(SyntaxToken* token, std::vector<StatementNode*> stmts);
    ~BlockStatementNode();

    /*
     * The list of statements in this code block.
     */
    std::vector<StatementNode*> statements() const;

    SyntaxKind kind() const override;
};

}

#endif