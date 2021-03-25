#ifndef SYNTAX_ASSIGNMENT_EXPRESSION_NODE_H
#define SYNTAX_ASSIGNMENT_EXPRESSION_NODE_H

#include "expression_node.h"
#include "operator_tokens.h"

namespace Syntax {

class AssignmentExpressionNode : public ExpressionNode {
private:
    ExpressionNode* lhs;
    SyntaxTokenType assignment_token;
    ExpressionNode* rhs;
public:
    AssignmentExpressionNode(ExpressionNode* id_exp, SyntaxTokenType assign, ExpressionNode* expr) :
        lhs(id_exp), assignment_token(assign), rhs(expr) {}
    
    ~AssignmentExpressionNode() {
        delete lhs;
        delete rhs;
    }

    /*
     * The expression containing the identifier.
     */
    ExpressionNode* identifier_expression() const { return lhs; }

    /*
     * The type of assignment expression being performed on
     * the identifier.
     */
    SyntaxTokenType assignment_type() const { return assignment_token; }

    /*
     * The expression that the identifier will be assigned.
     */
    ExpressionNode* expression() const { return rhs; }
};

}
#endif