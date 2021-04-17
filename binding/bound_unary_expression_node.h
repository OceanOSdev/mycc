#pragma once
#ifndef BOUND_UNARY_EXPRESSION_NODE_H
#define BOUND_UNARY_EXPRESSION_NODE_H

#include "bound_expression_node.h"

namespace Syntax {
    enum btokentype : int;
}

namespace Binding {

enum BoundUnaryOpKind {
    /* I Types */
    OnesComplement,
    
    /* N types */
    Negation,
    LogicalNegation,
};

using BoundUnaryOpKind = enum BoundUnaryOpKind;

class BoundUnaryOperatorNode {
private:
    BoundUnaryOpKind m_op_kind;
    const Symbols::TypeSymbol* m_right_type; 
    const Symbols::TypeSymbol* m_type;
    static BoundUnaryOperatorNode* BindITypeOperator(BoundUnaryOpKind opKind, const Symbols::TypeSymbol* right);
    static BoundUnaryOperatorNode* BindNTypeOperator(BoundUnaryOpKind opKind, const Symbols::TypeSymbol* right);
    BoundUnaryOperatorNode(BoundUnaryOpKind op, const Symbols::TypeSymbol* right_type, const Symbols::TypeSymbol* type);
public:
    static BoundUnaryOperatorNode* Bind(Syntax::btokentype syntax_token_type, const Symbols::TypeSymbol* right);
    
    /*
     * The type symbol for the expression.
     *
     * Called "right_type" to differentiate it from
     * the method that returns the unary op return type.
     */
    const Symbols::TypeSymbol* right_type() const;

    /*
     * The operator kind.
     */
    BoundUnaryOpKind op_kind() const;
    
    /*
     * The type that the unary expression operator returns.
     */
    const Symbols::TypeSymbol* type() const;
};

class BoundUnaryExpressionNode : public BoundExpressionNode {
private:
    BoundUnaryOperatorNode* m_op;
    BoundExpressionNode* m_expression;
public:
    BoundUnaryExpressionNode(BoundUnaryOperatorNode* op, BoundExpressionNode* expression);
    ~BoundUnaryExpressionNode();

    /*
     * The bound operator.
     */
    BoundUnaryOperatorNode* op() const;

    /*
     * The bound expression on the right-hand side of the operator.
     */
    BoundExpressionNode* expression() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

    /*
     * The type that this expression evaluates to.
     */
    const Symbols::TypeSymbol* type() const override;
};

}

#endif