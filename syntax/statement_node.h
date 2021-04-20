#pragma once
#ifndef SYNTAX_STATEMENT_NODE_H
#define SYNTAX_STATEMENT_NODE_H

#include "syntax_node.h"

namespace Syntax {
class SyntaxToken;
class StatementNode : public SyntaxNode {
public:
    StatementNode(SyntaxToken* token) : SyntaxNode(token) {}
};

}

#endif