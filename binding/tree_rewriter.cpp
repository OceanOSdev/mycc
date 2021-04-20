#include "tree_rewriter.h"

#include "bound_statement_node.h"
#include "bound_expression_statement_node.h"
#include "bound_error_expression_node.h"
#include "bound_block_statement_node.h"
#include "bound_return_statement_node.h"
#include "bound_empty_statement_node.h"
#include "bound_label_statement_node.h"
#include "bound_if_statement_node.h"
#include "bound_goto_statement_node.h"
#include "bound_for_statement_node.h"
#include "bound_do_while_statement_node.h"
#include "bound_while_statement_node.h"
#include "bound_label_statement_node.h"
#include "bound_goto_statement_node.h"
#include "bound_variable_group_declaration_node.h"
#include "bound_literal_val_expression_node.h"
#include "bound_index_expression_node.h"
#include "bound_struct_declaration_node.h"
#include "bound_variable_reference_expression_node.h"
#include "bound_member_access_expression_node.h"
#include "bound_cast_expression_node.h"
#include "bound_binary_expression_node.h"
#include "bound_unary_expression_node.h"
#include "bound_assignment_expression_node.h"
#include "bound_call_expression_node.h"
#include "bound_ternary_expression_node.h"
#include "bound_increment_expression_node.h"

#include <string>
#include <stdexcept>
#include <vector>


namespace Binding {


TreeRewriter::TreeRewriter() : m_label_count(0) {}

BoundLabel* TreeRewriter::generate_label() {
    ++m_label_count;
    std::string label = "L" + std::to_string(m_label_count);
    return new BoundLabel(label);
}

BoundStatementNode* TreeRewriter::rewrite_statement(BoundStatementNode* statement) {
    auto kind = statement->kind();
    switch (kind) {
        case BoundNodeKind::ExpressionStatement:
            return rewrite_expression_statement(dynamic_cast<BoundExpressionStatementNode*>(statement));
        case BoundNodeKind::BlockStatement:
            return rewrite_block_statement(dynamic_cast<BoundBlockStatementNode*>(statement));
        case BoundNodeKind::VariableGroupDeclaration:
            return rewrite_variable_group_declaration(dynamic_cast<BoundVariableGroupDeclarationNode*>(statement));
        case BoundNodeKind::IfStatement:
            return rewrite_if_statement(dynamic_cast<BoundIfStatementNode*>(statement));
        case BoundNodeKind::ForStatement:
            return rewrite_for_statement(dynamic_cast<BoundForStatementNode*>(statement));
        case BoundNodeKind::WhileStatement:
            return rewrite_while_statement(dynamic_cast<BoundWhileStatementNode*>(statement));
        case BoundNodeKind::DoWhileStatement:
            return rewrite_do_while_statement(dynamic_cast<BoundDoWhileStatementNode*>(statement));
        case BoundNodeKind::ReturnStatement:
            return rewrite_return_statement(dynamic_cast<BoundReturnStatementNode*>(statement));
        case BoundNodeKind::StructDeclaration:
            return rewrite_struct_declaration(dynamic_cast<BoundStructDeclarationNode*>(statement));
        case BoundNodeKind::LabelStatement:
            return rewrite_label_statement(dynamic_cast<BoundLabelStatementNode*>(statement));
        case BoundNodeKind::GotoStatement:
            return rewrite_goto_statement(dynamic_cast<BoundGotoStatementNode*>(statement));
        default:
            throw std::runtime_error("Unexpected bound node while rewriting statement.");
    }
}

BoundStatementNode* TreeRewriter::rewrite_expression_statement(BoundExpressionStatementNode* expression_statement) {
    auto expression = rewrite_expression(expression_statement->expression());
    if (expression == expression_statement->expression())
        return expression_statement;
    
    return new BoundExpressionStatementNode(expression);
}

BoundStatementNode* TreeRewriter::rewrite_block_statement(BoundBlockStatementNode* block_statement) {

}

BoundStatementNode* TreeRewriter::rewrite_variable_group_declaration(BoundVariableGroupDeclarationNode* var_group_decl) {

}

BoundStatementNode* TreeRewriter::rewrite_if_statement(BoundIfStatementNode* if_statement) {

}

BoundStatementNode* TreeRewriter::rewrite_for_statement(BoundForStatementNode* for_statement) {

}

BoundStatementNode* TreeRewriter::rewrite_while_statement(BoundWhileStatementNode* while_statement) {

}

BoundStatementNode* TreeRewriter::rewrite_do_while_statement(BoundDoWhileStatementNode* do_while_statement) {

}

BoundStatementNode* TreeRewriter::rewrite_return_statement(BoundReturnStatementNode* return_statement) {

}

BoundStatementNode* TreeRewriter::rewrite_struct_declaration(BoundStructDeclarationNode* struct_declaration) {

}

BoundStatementNode* TreeRewriter::rewrite_label_statement(BoundLabelStatementNode* label_statement) {
    return label_statement;
}

BoundStatementNode* TreeRewriter::rewrite_goto_statement(BoundGotoStatementNode* goto_statement) {
    return goto_statement;
}


BoundExpressionNode* TreeRewriter::rewrite_expression(BoundExpressionNode* expression) {
    auto kind = expression->kind();
    switch (kind) {
        case BoundNodeKind::AssignmentExpression:
            return rewrite_assignment_expression(dynamic_cast<BoundAssignmentExpressionNode*>(expression));
        case BoundNodeKind::BinaryExpression:
            return rewrite_binary_expression(dynamic_cast<BoundBinaryExpressionNode*>(expression));
        case BoundNodeKind::CallExpression:
            return rewrite_call_expression(dynamic_cast<BoundCallExpressionNode*>(expression));
        case BoundNodeKind::CastExpression:
            return rewrite_cast_expression(dynamic_cast<BoundCastExpressionNode*>(expression));
        case BoundNodeKind::DecrementExpression:
            return rewrite_decrement_expression(dynamic_cast<BoundDecrementExpressionNode*>(expression));
        case BoundNodeKind::IncrementExpression:
            return rewrite_increment_expression(dynamic_cast<BoundIncrementExpressionNode*>(expression));
        case BoundNodeKind::IndexExpression:
            return rewrite_index_expression(dynamic_cast<BoundIndexExpressionNode*>(expression));
        case BoundNodeKind::LiteralExpression:
            return rewrite_literal_val_expression(dynamic_cast<BoundLiteralValExpressionNode*>(expression));
        case BoundNodeKind::MemberAccessExpression:
            return rewrite_member_expression(dynamic_cast<BoundMemberAccessExpressionNode*>(expression));
        case BoundNodeKind::VariableReferenceExpression:
            return rewrite_var_reference_expression(dynamic_cast<BoundVariableReferenceExpressionNode*>(expression));
        case BoundNodeKind::TernaryExpression:
            return rewrite_ternary_expression(dynamic_cast<BoundTernaryExpressionNode*>(expression));
        case BoundNodeKind::UnaryExpression:
            return rewrite_unary_expression(dynamic_cast<BoundUnaryExpressionNode*>(expression));

    }
}

BoundExpressionNode* TreeRewriter::rewrite_assignment_expression(BoundAssignmentExpressionNode* assignment_expression) {
    auto expression = rewrite_expression(assignment_expression->expression());
    auto variable = rewrite_expression(assignment_expression->variable_reference());

    if (expression == assignment_expression->expression() && variable == assignment_expression->variable_reference())
        return assignment_expression;
    
    return new BoundAssignmentExpressionNode(assignment_expression->assignment_op_kind(), dynamic_cast<BoundVariableReferenceExpressionNode*>(variable), expression);
}

BoundExpressionNode* TreeRewriter::rewrite_binary_expression(BoundBinaryExpressionNode* binary_expression) {
    auto left = rewrite_expression(binary_expression->left());
    auto right = rewrite_expression(binary_expression->right());

    if (left == binary_expression->left() && right == binary_expression->right())
        return binary_expression;
    
    return new BoundBinaryExpressionNode(binary_expression->op(), left, right);
}

BoundExpressionNode* TreeRewriter::rewrite_call_expression(BoundCallExpressionNode* call_expression) {
    bool requires_rewrite = false;
    std::vector<BoundExpressionNode*> new_args;
    auto num_args = call_expression->arguments().size();
    for (std::vector<BoundExpressionNode*>::size_type i = 0; i < num_args; i++) {
        auto old_arg = call_expression->arguments()[i];
        auto new_arg = rewrite_expression(old_arg);
        if (old_arg != new_arg)
            requires_rewrite = true;
        
        new_args.push_back(new_arg);
    }

    if (!requires_rewrite) return call_expression;
    return new BoundCallExpressionNode(call_expression->function(), new_args);
}

BoundExpressionNode* TreeRewriter::rewrite_cast_expression(BoundCastExpressionNode* cast_expression) {
    auto expression = rewrite_expression(cast_expression->expression());

    if (expression == cast_expression->expression())
        return cast_expression;

    return new BoundCastExpressionNode(cast_expression->type(), expression);
}

BoundExpressionNode* TreeRewriter::rewrite_decrement_expression(BoundDecrementExpressionNode* decrement_expression) {
    auto expression = rewrite_expression(decrement_expression->expression());

    if (expression == decrement_expression->expression())
        return decrement_expression;

    return new BoundDecrementExpressionNode(decrement_expression->notation(), decrement_expression->expression());
}

BoundExpressionNode* TreeRewriter::rewrite_increment_expression(BoundIncrementExpressionNode* increment_expression) {
    auto expression = rewrite_expression(increment_expression->expression());

    if (expression == increment_expression->expression())
        return increment_expression;

    return new BoundIncrementExpressionNode(increment_expression->notation(), increment_expression->expression());
}

BoundExpressionNode* TreeRewriter::rewrite_index_expression(BoundIndexExpressionNode* index_expression) {
    auto expression = rewrite_expression(index_expression->expression());

    if (expression == index_expression->expression())
        return index_expression;
    
    return new BoundIndexExpressionNode(index_expression->variable_reference(), expression);
}

BoundExpressionNode* TreeRewriter::rewrite_literal_val_expression(BoundLiteralValExpressionNode* literal_val_expression) {
    return literal_val_expression;
}

BoundExpressionNode* TreeRewriter::rewrite_member_expression(BoundMemberAccessExpressionNode* member_expression) {
    auto encapsulated_ref = rewrite_expression(member_expression->encapsulated_variable_reference());
    auto encapsulating_ref = rewrite_expression(member_expression->encapsulating_variable_reference());

    if (encapsulated_ref == member_expression->encapsulated_variable_reference() &&
        encapsulating_ref == member_expression->encapsulating_variable_reference())
        return member_expression;
    
    return new BoundMemberAccessExpressionNode(encapsulated_ref, encapsulating_ref);
}

BoundExpressionNode* TreeRewriter::rewrite_var_reference_expression(BoundVariableReferenceExpressionNode* var_reference_expression) {
    return var_reference_expression;
}

BoundExpressionNode* TreeRewriter::rewrite_ternary_expression(BoundTernaryExpressionNode* ternary_expression) {
    // TODO: might have to rewrite as some weird intermediary thing.
    // but to get stuff off the ground I'll just only rewrite the inner expressions.
    auto conditional = rewrite_expression(ternary_expression->conditional());
    auto true_expr = rewrite_expression(ternary_expression->true_expr());
    auto false_expr = rewrite_expression(ternary_expression->false_expr());

    if (conditional == ternary_expression->conditional() &&
        true_expr == ternary_expression->true_expr() &&
        false_expr == ternary_expression->false_expr())
        return ternary_expression;

    return new BoundTernaryExpressionNode(ternary_expression->type(), conditional, true_expr, false_expr);
}

BoundExpressionNode* TreeRewriter::rewrite_unary_expression(BoundUnaryExpressionNode* unary_expression) {
    auto operand = rewrite_expression(unary_expression->expression());

    if (operand == unary_expression->expression())
        return unary_expression;

    return new BoundUnaryExpressionNode(unary_expression->op(), operand);
}




}