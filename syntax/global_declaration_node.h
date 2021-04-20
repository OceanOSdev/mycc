#pragma once
#ifndef SYNTAX_GLOBAL_DECLARATION_NODE_H
#define SYNTAX_GLOBAL_DECLARATION_NODE_H

#include "syntax_node.h"

namespace Syntax {
class SyntaxToken;
class GlobalDeclarationNode : public SyntaxNode {
public:
    GlobalDeclarationNode(SyntaxToken* token) : SyntaxNode(token) {}
};

}

#endif