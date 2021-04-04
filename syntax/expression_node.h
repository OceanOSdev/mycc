#pragma once
#ifndef SYNTAX_EXPRESSION_NODE_H
#define SYNTAX_EXPRESSION_NODE_H

#include "syntax_node.h"

namespace Syntax {
class SyntaxToken;
class ExpressionNode : public SyntaxNode {
protected:
    SyntaxToken* m_token;
public:
    ExpressionNode(SyntaxToken* token);
};

}

#endif