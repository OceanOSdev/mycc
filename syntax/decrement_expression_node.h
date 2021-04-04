#pragma once
#ifndef SYNTAX_DECREMENT_EXPRESSION_NODE_H
#define SYNTAX_DECREMENT_EXPRESSION_NODE_H

#include "expression_node.h"

namespace Syntax {

class DecrementExpressionNode : public ExpressionNode {
private:
    ExpressionNode* id_expr;
    bool is_post_decrement;
public:
    DecrementExpressionNode(ExpressionNode* id, bool post);

    ~DecrementExpressionNode();

    /*
     * The expression containing the identifier.
     */
    ExpressionNode* identifier_expression() const;

    /*
     * Indicates whether we are post or pre decrementing.
     * 
     * Post-decrement: true.
     * 
     * Pre-decrement: false.
     */
    bool is_post() const;
};

}

#endif