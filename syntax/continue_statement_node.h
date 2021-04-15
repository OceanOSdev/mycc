#pragma once
#ifndef SYNTAX_CONTINUE_STATEMENT_NODE_H
#define SYNTAX_CONTINUE_STATEMENT_NODE_H

#include "statement_node.h"

namespace Syntax {
class SyntaxToken;
class ContinueStatementNode : public StatementNode {
public:
    ContinueStatementNode(SyntaxToken* token);
    ~ContinueStatementNode();

    SyntaxKind kind() const override;
};

}

#endif