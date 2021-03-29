#include "decrement_expression_node.h"

namespace Syntax {

DecrementExpressionNode::DecrementExpressionNode(ExpressionNode* id, bool post) :
    id_expr(id), is_post_decrement(post) { }

DecrementExpressionNode::~DecrementExpressionNode() {
    delete id_expr;
}

/*
 * The expression containing the identifier.
 */
ExpressionNode* DecrementExpressionNode::identifier_expression() const { 
    return id_expr; 
}

/*
 * Indicates whether we are post or pre decrementing.
 * 
 * Post-decrement: true.
 * 
 * Pre-decrement: false.
 */
bool DecrementExpressionNode::is_post() const { 
    return is_post_decrement; 
}

}