#include "member_expression_node.h"

namespace Syntax {

MemberExpressionNode::MemberExpressionNode(ExpressionNode* e, ExpressionNode* m) : 
    lhs(e), rhs(m) {}

MemberExpressionNode::~MemberExpressionNode() { 
    delete lhs;
    delete rhs; 
}

/*
 * The expression of the encapsulating type.
 */
ExpressionNode* MemberExpressionNode::encapsulator() const { 
    return lhs; 
}

/*
 * The expression of the member of the encapsulating type.
 */
ExpressionNode* MemberExpressionNode::member() const { 
    return rhs; 
}


}