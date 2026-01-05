#pragma once
#ifndef BOUND_GLOBAL_STATEMENT_NODE_H
#define BOUND_GLOBAL_STATEMENT_NODE_H

#include "bound_global_declaration_node.h"

namespace Binding {
class BoundStatementNode;

class BoundGlobalStatementNode : public BoundGlobalDeclarationNode {
private:
  BoundStatementNode *m_statement;

public:
  BoundGlobalStatementNode(BoundStatementNode *statement);

  /*
   * The statement that this bound class wraps around.
   */
  BoundStatementNode *statement() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;
};

} // namespace Binding

#endif