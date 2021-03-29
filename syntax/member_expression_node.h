#ifndef SYNTAX_MEMBER_EXPRESSION_NODE_H
#define SYNTAX_MEMBER_EXPRESSION_NODE_H

#include <string>
#include "expression_node.h"

namespace Syntax {

class MemberExpressionNode : public ExpressionNode {
private:
    ExpressionNode* lhs;
    ExpressionNode* rhs;
public:
    MemberExpressionNode(ExpressionNode* e, ExpressionNode* m);

    ~MemberExpressionNode();

    /*
     * The expression of the encapsulating type.
     */
    ExpressionNode* encapsulator() const;

    /*
     * The expression of the member of the encapsulating type.
     */
    ExpressionNode* member() const;
};

}

#endif