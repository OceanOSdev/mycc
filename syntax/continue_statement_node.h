#ifndef SYNTAX_CONTINUE_STATEMENT_NODE_H
#define SYNTAX_CONTINUE_STATEMENT_NODE_H

#include "statement_node.h"

namespace Syntax {

class ContinueStatementNode : public StatementNode {
public:
    ContinueStatementNode() { }
    ~ContinueStatementNode() { }
};

}

#endif