#pragma once
#ifndef SYNTAX_GLOBAL_VARIABLE_GROUP_DECLARATION_NODE_H
#define SYNTAX_GLOBAL_VARIABLE_GROUP_DECLARATION_NODE_H

#include "global_declaration_node.h"
#include "variable_group_declaration_node.h"
#include <vector>

namespace Syntax {
class SyntaxToken;
class GlobalVariableGroupDeclarationNode : public GlobalDeclarationNode {
private:
  VariableGroupDeclarationNode *m_variable_group;

public:
  GlobalVariableGroupDeclarationNode(SyntaxToken *token,
                                     VariableGroupDeclarationNode *var_group);

  ~GlobalVariableGroupDeclarationNode();

  /*
   * The variable group declarations.
   */
  VariableGroupDeclarationNode *variable_group() const;

  SyntaxKind kind() const override;
};

} // namespace Syntax

#endif