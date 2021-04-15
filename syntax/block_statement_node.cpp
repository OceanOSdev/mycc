#include "block_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

BlockStatementNode::BlockStatementNode(SyntaxToken* token, std::vector<StatementNode*> stmts) : 
    StatementNode(token), stmt_nodes(stmts) { }

BlockStatementNode::~BlockStatementNode() {
    std::vector<StatementNode*>::iterator stmt_iter;
    for (stmt_iter = stmt_nodes.begin(); stmt_iter != stmt_nodes.end(); stmt_iter++)
        delete *stmt_iter; 
}

/*
 * The list of statements in this code block.
 */
std::vector<StatementNode*> BlockStatementNode::statements() const { 
    return stmt_nodes; 
}

SyntaxKind BlockStatementNode::kind() const {
    return SyntaxKind::BlockStatement;
}

}