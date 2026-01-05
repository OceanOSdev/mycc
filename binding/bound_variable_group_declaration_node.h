#pragma once
#ifndef BOUND_VARIABLE_DECLARATION_GROUP_NODE_H
#define BOUND_VARIABLE_DECLARATION_GROUP_NODE_H

#include "bound_statement_node.h"
#include <vector>

namespace Symbols {
class VariableSymbol;
}

namespace Binding {

class BoundExpressionNode;

class BoundVariableDeclarationNode : public BoundStatementNode {
private:
  Symbols::VariableSymbol *m_variable_symbol;
  BoundExpressionNode *m_initializer;

public:
  BoundVariableDeclarationNode(Symbols::VariableSymbol *variable_symbol,
                               BoundExpressionNode *initializer);

  Symbols::VariableSymbol *variable_symbol() const;

  /*
   * The bound expression that represent this variable's
   * initial value at declaration.
   */
  BoundExpressionNode *initializer() const;

  bool is_initialized() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;
};

class BoundVariableGroupDeclarationNode : public BoundStatementNode {
private:
  std::vector<BoundVariableDeclarationNode *> m_variable_declarations;

public:
  BoundVariableGroupDeclarationNode(
      std::vector<BoundVariableDeclarationNode *> variable_declarations);

  std::vector<BoundVariableDeclarationNode *> variable_declarations() const;

  /*
   * An enum representing the kind of bound node this is
   */
  BoundNodeKind kind() const override;
};

} // namespace Binding

#endif