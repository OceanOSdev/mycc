#pragma once
#ifndef SYNTAX_RETURN_STATEMENT_NODE_H
#define SYNTAX_RETURN_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {

class ReturnStatementNode : public StatementNode {
    ExpressionNode* expr;
public:
    ReturnStatementNode(ExpressionNode* e);
    ~ReturnStatementNode();

    /*
     * The expression to return.
     */
    ExpressionNode* expression() const;

    /*
     * Whether or not this is a statement of the form "return;"
     */
    bool is_empty_return() const;
};

}

#endif