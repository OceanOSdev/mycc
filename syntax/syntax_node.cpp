#include "syntax_node.h"
#include "syntax_token.h"
namespace Syntax {

SyntaxNode::SyntaxNode(SyntaxToken *token) : m_token(token) {}

SyntaxToken *SyntaxNode::token() const { return m_token; }

} // namespace Syntax