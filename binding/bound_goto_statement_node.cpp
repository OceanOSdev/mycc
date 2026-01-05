#include "bound_goto_statement_node.h"
#include "bound_label_statement_node.h"

namespace Binding {

BoundGotoStatementNode::BoundGotoStatementNode(BoundLabel *label)
    : m_label(label) {}

BoundLabel *BoundGotoStatementNode::label() const { return m_label; }

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundGotoStatementNode::kind() const {
  return BoundNodeKind::GotoStatement;
}

} // namespace Binding