#pragma once
#ifndef SYNTAX_FORMAL_PARAMETER_NODE_H
#define SYNTAX_FORMAL_PARAMETER_NODE_H

#include "syntax_node.h"
#include <string>

namespace Syntax {
class SyntaxToken;
class FormalParameterNode : public SyntaxNode {
private:
  std::string m_type_identifier;
  std::string m_param_name;
  bool m_is_struct;
  bool m_is_const;
  bool m_is_array;

public:
  FormalParameterNode(SyntaxToken *token, std::string id, std::string name,
                      bool is_struct = false, bool is_const = false,
                      bool is_array = false);

  ~FormalParameterNode();

  /*
   * The name of the type that the parameter is.
   */
  std::string type_identifier() const;

  /*
   * The name of the parameter.
   */
  std::string param_name() const;

  /*
   * Whether the parameter is a struct type or not.
   */
  bool is_struct() const;

  /*
   * Whether the parameter is a constant type or not.
   */
  bool is_const() const;

  /*
   * Whether the parameter is an array or not.
   */
  bool is_array() const;

  SyntaxKind kind() const override;
};

} // namespace Syntax

#endif