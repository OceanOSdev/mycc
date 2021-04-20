#include "bound_block_statement_node.h"

namespace Binding {

BoundBlockStatementNode::BoundBlockStatementNode(std::vector<BoundStatementNode*> statements) :
    m_statements(statements) {}

/*
 * The list of statements encapsulated by the block.
 */
std::vector<BoundStatementNode*> BoundBlockStatementNode::statements() const {
    return m_statements;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundBlockStatementNode::kind() const {
    return BoundNodeKind::BlockStatement;
}

}