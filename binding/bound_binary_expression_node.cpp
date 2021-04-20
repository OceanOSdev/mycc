#include "bound_binary_expression_node.h"
#include "../symbols/type_symbol.h"
#include "../syntax/syntax_token.h"

namespace Binding {

BoundBinaryOperatorNode::BoundBinaryOperatorNode(BoundBinaryOpKind op, const Symbols::TypeSymbol* left, const Symbols::TypeSymbol* right, const Symbols::TypeSymbol* type) :
    m_op_kind(op), m_left_type(left), m_right_type(right), m_type(type) {}

BoundBinaryOperatorNode* BoundBinaryOperatorNode::BindITypeOperator(BoundBinaryOpKind opKind, const Symbols::TypeSymbol* left, const Symbols::TypeSymbol* right) {
    if (!left->attributes().is_integer_type || !right->attributes().is_integer_type)
        return nullptr;
    
    auto op_ret_type = Symbols::TypeSymbol::get_wider_type(left, right);
    if (op_ret_type == nullptr) return nullptr;

    return new BoundBinaryOperatorNode(opKind, left, right, op_ret_type);
}

BoundBinaryOperatorNode* BoundBinaryOperatorNode::BindNTypeOperator(BoundBinaryOpKind opKind, const Symbols::TypeSymbol* left, const Symbols::TypeSymbol* right) {
    if (!left->attributes().is_numeric_type || !right->attributes().is_numeric_type)
        return nullptr;
    
    auto op_ret_type = Symbols::TypeSymbol::get_wider_type(left, right);
    if (op_ret_type == nullptr) return nullptr;
    
    return new BoundBinaryOperatorNode(opKind, left, right, op_ret_type);
}

BoundBinaryOperatorNode* BoundBinaryOperatorNode::BindNTypeCompOperator(BoundBinaryOpKind opKind, const Symbols::TypeSymbol* left, const Symbols::TypeSymbol* right) {
    if (!left->attributes().is_numeric_type || !right->attributes().is_numeric_type)
        return nullptr;
    
    const Symbols::TypeSymbol* type = &Symbols::TypeSymbol::Char;
    if (left->attributes().is_const && right->attributes().is_const)
        type = type->as_const_type();
    return new BoundBinaryOperatorNode(opKind, left, right, type);
}

BoundBinaryOperatorNode* BoundBinaryOperatorNode::Bind(Syntax::btokentype syntax_token_type, const Symbols::TypeSymbol* left, const Symbols::TypeSymbol* right) {
    BoundBinaryOpKind opKind;

    // array types technically might have the attributes numeric and integer
    // so we just make sure neither types are arrays here first
    if (left->attributes().is_array || right->attributes().is_array)
        return nullptr;

    const Symbols::TypeSymbol* left_type = left;
    const Symbols::TypeSymbol* right_type = right;

    if (left->attributes().is_const ^ right->attributes().is_const) {
        left_type = left->as_mutable_type();
        right_type = right->as_mutable_type();
    }
        
    switch (syntax_token_type) {
        case Syntax::token_type_t::MOD:
            opKind = BoundBinaryOpKind::Modulo;
            break;
        case Syntax::token_type_t::AMP:
            opKind = BoundBinaryOpKind::BitwiseAnd;
            break;
        case Syntax::token_type_t::PIPE:
            opKind = BoundBinaryOpKind::BitwiseOr;
            break;
        case Syntax::token_type_t::PLUS:
            opKind = BoundBinaryOpKind::Addition;
            break;
        case Syntax::token_type_t::MINUS:
            opKind = BoundBinaryOpKind::Subtraction;
            break;
        case Syntax::token_type_t::STAR:
            opKind = BoundBinaryOpKind::Multiplication;
            break;
        case Syntax::token_type_t::SLASH:
            opKind = BoundBinaryOpKind::Division;
            break;
        case Syntax::token_type_t::EQUALS:
            opKind = BoundBinaryOpKind::Equals;
            break;
        case Syntax::token_type_t::NEQUAL:
            opKind = BoundBinaryOpKind::NotEquals;
            break;
        case Syntax::token_type_t::LT:
            opKind = BoundBinaryOpKind::LessThan;
            break;
        case Syntax::token_type_t::LE:
            opKind = BoundBinaryOpKind::LessThanOrEquals;
            break;
        case Syntax::token_type_t::GT:
            opKind = BoundBinaryOpKind::GreaterThan;
            break;
        case Syntax::token_type_t::GE:
            opKind = BoundBinaryOpKind::GreaterThanOrEquals;
            break;
        case Syntax::token_type_t::DAMP:
            opKind = BoundBinaryOpKind::LogicalAnd;
            break;
        case Syntax::token_type_t::DPIPE:
            opKind = BoundBinaryOpKind::LogicalOr;
            break;
        default:
            return nullptr;
    }

    if (opKind == BoundBinaryOpKind::Modulo ||
        opKind == BoundBinaryOpKind::BitwiseAnd ||
        opKind == BoundBinaryOpKind::BitwiseOr)
            return BindITypeOperator(opKind, left_type, right_type);
    
    if (opKind == BoundBinaryOpKind::Addition ||
        opKind == BoundBinaryOpKind::Subtraction ||
        opKind == BoundBinaryOpKind::Multiplication ||
        opKind == BoundBinaryOpKind::Division) 
            return BindNTypeOperator(opKind, left_type, right_type);
    
    return BindNTypeCompOperator(opKind, left_type, right_type);
    
}
    
/*
 * The type symbol for the left side of the expression.
 */
const Symbols::TypeSymbol* BoundBinaryOperatorNode::left_type() const {
    return m_left_type;
}

/*
 * The operator kind.
 */
BoundBinaryOpKind BoundBinaryOperatorNode::op_kind() const {
    return m_op_kind;
}

/*
 * The type symbol for the right side of the expression.
 */
const Symbols::TypeSymbol* BoundBinaryOperatorNode::right_type() const {
    return m_right_type;
}

/*
 * The type that the binary expression operator returns.
 */
const Symbols::TypeSymbol* BoundBinaryOperatorNode::type() const {
    return m_type;
}


BoundBinaryExpressionNode::BoundBinaryExpressionNode(BoundBinaryOperatorNode* op, BoundExpressionNode* left, BoundExpressionNode* right) :
    m_op(op), m_left(left), m_right(right) {}

BoundBinaryExpressionNode::~BoundBinaryExpressionNode() {

}

/*
 * The bound operator.
 */
BoundBinaryOperatorNode* BoundBinaryExpressionNode::op() const {
    return m_op;
}

/*
 * The bound expression on the left-hand side of the operator.
 */
BoundExpressionNode* BoundBinaryExpressionNode::left() const {
    return m_left;
}

/*
 * The bound expression on the right-hand side of the operator.
 */
BoundExpressionNode* BoundBinaryExpressionNode::right() const {
    return m_right;
}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundBinaryExpressionNode::kind() const {
    return BoundNodeKind::BinaryExpression;
}

/*
 * The type that this expression evaluates to.
 */
const Symbols::TypeSymbol* BoundBinaryExpressionNode::type() const {
    return m_op->type();
}

}
