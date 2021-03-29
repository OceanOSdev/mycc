#ifndef SYNTAX_BREAK_STATEMENT_NODE_H
#define SYNTAX_BREAK_STATEMENT_NODE_H

#include "statement_node.h"

namespace Syntax {

class BreakStatementNode : public StatementNode {
public:
    BreakStatementNode();
    ~BreakStatementNode();
};

}

#endif