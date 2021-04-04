#pragma once
#ifndef SYNTAX_INCREMENT_EXPRESSION_NODE_H
#define SYNTAX_INCREMENT_EXPRESSION_NODE_H

#include "expression_node.h"

namespace Syntax {

class IncrementExpressionNode : public ExpressionNode {
private:
    ExpressionNode* id_expr;
    bool is_post_increment;
public:
    IncrementExpressionNode(ExpressionNode* id, bool post);

    ~IncrementExpressionNode();

    /*
     * The expression containing the identifier.
     */
    ExpressionNode* identifier_expression() const;

    /*
     * Indicates whether we are post or pre incrementing.
     * 
     * Post-increment: true.
     * 
     * Pre-increment: false.
     */
    bool is_post() const;
};

}

#endif