#pragma once
#ifndef SYNTAX_NODE_H
#define SYNTAX_NODE_H

namespace Syntax {
class SyntaxToken;
enum SyntaxKind : int;

class SyntaxNode {
protected:
  SyntaxToken *m_token;

public:
  SyntaxNode(SyntaxToken *token);
  virtual ~SyntaxNode() = default;

  /*
   * What kind of syntax node this is.
   */
  virtual SyntaxKind kind() const = 0;

  SyntaxToken *token() const;
};

} // namespace Syntax

#endif