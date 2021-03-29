#include "return_statement_node.h"

namespace Syntax {

ReturnStatementNode::ReturnStatementNode(ExpressionNode* e) : 
    expr(e) {}

ReturnStatementNode::~ReturnStatementNode() {
    if (!is_empty_return())
        delete expr;
}

/*
 * The expression to return.
 */
ExpressionNode* ReturnStatementNode::expression() const { 
    return expr;
}

/*
 * Whether or not this is a statement of the form "return;"
 */
bool ReturnStatementNode::is_empty_return() const { 
    return expr == nullptr;
}

}