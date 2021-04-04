#pragma once
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
    AssignmentExpressionNode(ExpressionNode* id_exp, SyntaxTokenType assign, ExpressionNode* expr);
    
    ~AssignmentExpressionNode();

    /*
     * The expression containing the identifier.
     */
    ExpressionNode* identifier_expression() const;

    /*
     * The type of assignment expression being performed on
     * the identifier.
     */
    SyntaxTokenType assignment_type() const;

    /*
     * The expression that the identifier will be assigned.
     */
    ExpressionNode* expression() const;
};

}
#endif