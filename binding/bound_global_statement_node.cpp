#include "bound_global_statement_node.h"
#include "bound_statement_node.h"

namespace Binding {

BoundGlobalStatementNode::BoundGlobalStatementNode(
    BoundStatementNode *statement)
    : m_statement(statement) {}

/*
 * The statement that this bound class wraps around.
 */
BoundStatementNode *BoundGlobalStatementNode::statement() const {
  return m_statement;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundGlobalStatementNode::kind() const {
  return BoundNodeKind::GlobalStatement;
}

} // namespace Binding