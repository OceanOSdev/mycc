#include "decrement_expression_node.h"
#include "syntax_kind.h"

namespace Syntax {

DecrementExpressionNode::DecrementExpressionNode(SyntaxToken* token, ExpressionNode* id, bool post) :
    ExpressionNode(token), id_expr(id), is_post_decrement(post) { }

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

SyntaxKind DecrementExpressionNode::kind() const {
    return SyntaxKind::DecrementExpression;
}

}