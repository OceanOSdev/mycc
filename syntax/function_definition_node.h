#pragma once
#ifndef SYNTAX_FUNCTION_DEFINITION_NODE_H
#define SYNTAX_FUNCTION_DEFINITION_NODE_H

#include "function_declaration_node.h"
#include "global_declaration_node.h"
#include "local_declaration_node.h"
#include "statement_node.h"
#include <string>
#include <vector>

namespace Syntax {
class SyntaxToken;
class FunctionDefinitionNode : public GlobalDeclarationNode {
private:
  FunctionDeclarationNode *m_func_decl;
  std::vector<Syntax::LocalDeclarationNode *> m_local_declarations;
  std::vector<Syntax::StatementNode *> m_statements;

public:
  FunctionDefinitionNode(SyntaxToken *token, FunctionDeclarationNode *func,
                         std::vector<Syntax::LocalDeclarationNode *> local_decs,
                         std::vector<Syntax::StatementNode *> statements);

  ~FunctionDefinitionNode();

  /*
   * The function that this definition is declaring.
   */
  FunctionDeclarationNode *function_declaration() const;

  /*
   * The declarations in the function.
   */
  std::vector<Syntax::LocalDeclarationNode *> local_declarations() const;

  /*
   * The statements in this function after the declarations.
   */
  std::vector<Syntax::StatementNode *> statements() const;

  SyntaxKind kind() const override;
};

} // namespace Syntax

#endif