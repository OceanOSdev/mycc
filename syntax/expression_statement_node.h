#ifndef SYNTAX_EXPRESSION_STATEMENT_NODE_H
#define SYNTAX_EXPRESSION_STATEMENT_NODE_H

#include "statement_node.h"
#include "expression_node.h"

namespace Syntax {

class ExpressionStatementNode : public StatementNode {
private:
    ExpressionNode* expr;
public:
    ExpressionStatementNode(ExpressionNode* e) : expr(e) {}
    ~ExpressionStatementNode() { delete expr; }

    /*
     * The expression contained in this statement.
     */
    ExpressionNode* expression() const { return expr; }
};

}

#endif