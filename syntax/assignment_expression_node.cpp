#include "assignment_expression_node.h"

namespace Syntax {

AssignmentExpressionNode::AssignmentExpressionNode(ExpressionNode* id_exp, SyntaxTokenType assign, ExpressionNode* expr) :
    lhs(id_exp), assignment_token(assign), rhs(expr) {}

AssignmentExpressionNode::~AssignmentExpressionNode() {
    delete lhs;
    delete rhs;
}

/*
 * The expression containing the identifier.
 */
ExpressionNode* AssignmentExpressionNode::identifier_expression() const { 
    return lhs; 
}

/*
 * The type of assignment expression being performed on
 * the identifier.
 */
SyntaxTokenType AssignmentExpressionNode::assignment_type() const { 
    return assignment_token; 
}

/*
 * The expression that the identifier will be assigned.
 */
ExpressionNode* AssignmentExpressionNode::expression() const { 
    return rhs; 
}


}