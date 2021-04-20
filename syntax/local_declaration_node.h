#pragma once
#ifndef SYNTAX_LOCAL_DECLARATION_NODE_H
#define SYNTAX_LOCAL_DECLARATION_NODE_H

#include "statement_node.h"

namespace Syntax {
class SyntaxToken;
class LocalDeclarationNode : public StatementNode {
public:
    LocalDeclarationNode(SyntaxToken* token) : StatementNode(token) {}
};

}

#endif