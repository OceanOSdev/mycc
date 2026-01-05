#include "bound_label_statement_node.h"

namespace Binding {

BoundLabel::BoundLabel(std::string name) : m_name(name) {}

std::string BoundLabel::name() const { return m_name; }

BoundLabelStatementNode::BoundLabelStatementNode(BoundLabel *label)
    : m_label(label) {}

BoundLabel *BoundLabelStatementNode::label() const { return m_label; }

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundLabelStatementNode::kind() const {
  return BoundNodeKind::LabelStatement;
}

} // namespace Binding