#include "bound_unary_expression_node.h"
#include "../symbols/type_symbol.h"
#include "../syntax/syntax_token.h"

namespace Binding {

BoundUnaryOperatorNode* BoundUnaryOperatorNode::BindITypeOperator(BoundUnaryOpKind opKind, const Symbols::TypeSymbol* right) {
    if (!right->attributes().is_integer_type)
        return nullptr;
    
    return new BoundUnaryOperatorNode(opKind,right,right);
}

BoundUnaryOperatorNode* BoundUnaryOperatorNode::BindNTypeOperator(BoundUnaryOpKind opKind, const Symbols::TypeSymbol* right) {
    if (!right->attributes().is_numeric_type)
        return nullptr;
    
    return new BoundUnaryOperatorNode(opKind,right,right);
}

BoundUnaryOperatorNode::BoundUnaryOperatorNode(BoundUnaryOpKind op, const Symbols::TypeSymbol* right_type, const Symbols::TypeSymbol* type) :
    m_op_kind(op), m_right_type(right_type), m_type(type) {}

BoundUnaryOperatorNode* BoundUnaryOperatorNode::Bind(Syntax::btokentype syntax_token_type, const Symbols::TypeSymbol* right) {
    BoundUnaryOpKind opKind;

    // array types technically might have the attributes numeric and integer
    // so we just make sure neither types are arrays here first
    if (right->attributes().is_array)
        return nullptr;
    
    switch (syntax_token_type) {
        case Syntax::token_type_t::TILDE:
            opKind = BoundUnaryOpKind::OnesComplement;
            break;
        case Syntax::token_type_t::MINUS:
            opKind = BoundUnaryOpKind::Negation;
            break;
        case Syntax::token_type_t::BANG:
            opKind = BoundUnaryOpKind::LogicalNegation;
            break;
        default:
            return nullptr;
    }

    if (opKind == BoundUnaryOpKind::OnesComplement)
        return BindITypeOperator(opKind, right);
    return BindNTypeOperator(opKind, right);
}
    
/*
 * The type symbol for the expression.
 *
 * Called "right_type" to differentiate it from
 * the method that returns the unary op return type.
 */
const Symbols::TypeSymbol* BoundUnaryOperatorNode::right_type() const {
    return m_right_type;
}

/*
 * The operator kind.
 */
BoundUnaryOpKind BoundUnaryOperatorNode::op_kind() const {
    return m_op_kind;
}
    
/*
 * The type that the unary expression operator returns.
 */
const Symbols::TypeSymbol* BoundUnaryOperatorNode::type() const {
    return m_type;
}


BoundUnaryExpressionNode::BoundUnaryExpressionNode(BoundUnaryOperatorNode* op, BoundExpressionNode* expression) :
    m_op(op), m_expression(expression) {}

BoundUnaryExpressionNode::~BoundUnaryExpressionNode() {

}

/*
 * The bound operator.
 */
BoundUnaryOperatorNode* BoundUnaryExpressionNode::op() const {
    return m_op;
}

/*
 * The bound expression on the right-hand side of the operator.
 */
BoundExpressionNode* BoundUnaryExpressionNode::expression() const {
    return m_expression;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundUnaryExpressionNode::kind() const {
    return BoundNodeKind::UnaryExpression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundUnaryExpressionNode::type() const {
    return m_op->type();
}

}