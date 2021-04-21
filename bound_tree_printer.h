#pragma once
#ifndef BOUND_TREE_PRINTER_H
#define BOUND_TREE_PRINTER_H

namespace Binding {

// Forward Decs
class BoundGlobalDeclarationNode;

class BoundStatementNode;
class BoundExpressionNode;
class BoundExpressionStatementNode;
class BoundBlockStatementNode;
class BoundVariableGroupDeclarationNode;
class BoundVariableDeclarationNode;
class BoundIfStatementNode;
class BoundForStatementNode;
class BoundWhileStatementNode;
class BoundDoWhileStatementNode;
class BoundReturnStatementNode;
class BoundStructDeclarationNode;
class BoundLabelStatementNode;
class BoundGotoStatementNode;
class BoundConditionalGotoStatementNode;

class BoundExpressionNode;
class BoundAssignmentExpressionNode;
class BoundBinaryExpressionNode;
class BoundCallExpressionNode;
class BoundCastExpressionNode;
class BoundDecrementExpressionNode;
class BoundIncrementExpressionNode;
class BoundIndexExpressionNode;
class BoundLiteralValExpressionNode;
class BoundMemberAccessExpressionNode;
class BoundVariableReferenceExpressionNode;
class BoundTernaryExpressionNode;
class BoundUnaryExpressionNode;

}

class BoundTreePrinter {
private:
    void print_statement(Binding::BoundStatementNode* statement);
    void print_expression_statement(Binding::BoundExpressionStatementNode* expression_statement);
    void print_block_statement(Binding::BoundBlockStatementNode* block_statement);
    void print_variable_group_declaration(Binding::BoundVariableGroupDeclarationNode* var_group_decl);
    void print_variable_declaration(Binding::BoundVariableDeclarationNode* var_decl);
    void print_if_statement(Binding::BoundIfStatementNode* if_statement);
    void print_for_statement(Binding::BoundForStatementNode* for_statement);
    void print_while_statement(Binding::BoundWhileStatementNode* while_statement);
    void print_do_while_statement(Binding::BoundDoWhileStatementNode* do_while_statement);
    void print_return_statement(Binding::BoundReturnStatementNode* return_statement);
    void print_struct_declaration(Binding::BoundStructDeclarationNode* struct_declaration);
    void print_label_statement(Binding::BoundLabelStatementNode* label_statement);
    void print_goto_statement(Binding::BoundGotoStatementNode* goto_statement);
    void print_conditional_goto_statement(Binding::BoundConditionalGotoStatementNode* conditional_goto_statement);

    void print_expression(Binding::BoundExpressionNode* expression);
    void print_assignment_expression(Binding::BoundAssignmentExpressionNode* assignment_expression);
    void print_binary_expression(Binding::BoundBinaryExpressionNode* binary_expression);
    void print_call_expression(Binding::BoundCallExpressionNode* call_expression);
    void print_cast_expression(Binding::BoundCastExpressionNode* cast_expression);
    void print_decrement_expression(Binding::BoundDecrementExpressionNode* decrement_expression);
    void print_increment_expression(Binding::BoundIncrementExpressionNode* increment_expression);
    void print_index_expression(Binding::BoundIndexExpressionNode* index_expression);
    void print_literal_val_expression(Binding::BoundLiteralValExpressionNode* literal_val_expression);
    void print_member_expression(Binding::BoundMemberAccessExpressionNode* member_expression);
    void print_var_reference_expression(Binding::BoundVariableReferenceExpressionNode* var_reference_expression);
    void print_ternary_expression(Binding::BoundTernaryExpressionNode* ternary_expression);
    void print_unary_expression(Binding::BoundUnaryExpressionNode* unary_expression);


};

#endif