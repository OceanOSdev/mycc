#pragma once
#ifndef BOUND_GOTO_STATEMENT_NODE_H
#define BOUND_GOTO_STATEMENT_NODE_H

#include "bound_statement_node.h"

namespace Binding {
class BoundLabel;

class BoundGotoStatementNode : public BoundStatementNode {
private:
  BoundLabel *m_label;

public:
  BoundGotoStatementNode(BoundLabel *label);

  BoundLabel *label() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;
};

} // namespace Binding

#endif