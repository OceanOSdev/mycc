#pragma once
#ifndef TREE_REWRITER_H
#define TREE_REWRITER_H

#include <vector>

namespace Binding {

// Forward Decs
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
class BoundGlobalDeclarationNode;
class BoundGlobalStatementNode;

class BoundLabel;

class TreeRewriter {
private:
    int m_label_count;
    TreeRewriter();
    TreeRewriter(int label_offset);

    static BoundBlockStatementNode* flatten(BoundStatementNode* statement);

    BoundLabel* generate_label();
    
    static BoundGlobalDeclarationNode* rewrite_global_statement(BoundGlobalStatementNode* bound_global_statement);

    BoundStatementNode* rewrite_statement(BoundStatementNode* statement);
    BoundStatementNode* rewrite_expression_statement(BoundExpressionStatementNode* expression_statement);
    BoundStatementNode* rewrite_block_statement(BoundBlockStatementNode* block_statement);
    BoundStatementNode* rewrite_variable_group_declaration(BoundVariableGroupDeclarationNode* var_group_decl);
    BoundStatementNode* rewrite_variable_declaration(BoundVariableDeclarationNode* var_decl);
    BoundStatementNode* rewrite_if_statement(BoundIfStatementNode* if_statement);
    BoundStatementNode* rewrite_for_statement(BoundForStatementNode* for_statement);
    BoundStatementNode* rewrite_while_statement(BoundWhileStatementNode* while_statement);
    BoundStatementNode* rewrite_do_while_statement(BoundDoWhileStatementNode* do_while_statement);
    BoundStatementNode* rewrite_return_statement(BoundReturnStatementNode* return_statement);
    BoundStatementNode* rewrite_struct_declaration(BoundStructDeclarationNode* struct_declaration);
    BoundStatementNode* rewrite_label_statement(BoundLabelStatementNode* label_statement);
    BoundStatementNode* rewrite_goto_statement(BoundGotoStatementNode* goto_statement);
    BoundStatementNode* rewrite_conditional_goto_statement(BoundConditionalGotoStatementNode* conditional_goto_statement);

    BoundExpressionNode* rewrite_expression(BoundExpressionNode* expression);
    BoundExpressionNode* rewrite_assignment_expression(BoundAssignmentExpressionNode* assignment_expression);
    BoundExpressionNode* rewrite_binary_expression(BoundBinaryExpressionNode* binary_expression);
    BoundExpressionNode* rewrite_call_expression(BoundCallExpressionNode* call_expression);
    BoundExpressionNode* rewrite_cast_expression(BoundCastExpressionNode* cast_expression);
    BoundExpressionNode* rewrite_decrement_expression(BoundDecrementExpressionNode* decrement_expression);
    BoundExpressionNode* rewrite_increment_expression(BoundIncrementExpressionNode* increment_expression);
    BoundExpressionNode* rewrite_index_expression(BoundIndexExpressionNode* index_expression);
    BoundExpressionNode* rewrite_literal_val_expression(BoundLiteralValExpressionNode* literal_val_expression);
    BoundExpressionNode* rewrite_member_expression(BoundMemberAccessExpressionNode* member_expression);
    BoundExpressionNode* rewrite_var_reference_expression(BoundVariableReferenceExpressionNode* var_reference_expression);
    BoundExpressionNode* rewrite_ternary_expression(BoundTernaryExpressionNode* ternary_expression);
    BoundExpressionNode* rewrite_unary_expression(BoundUnaryExpressionNode* unary_expression);

public:

    static BoundBlockStatementNode* rewrite(BoundStatementNode* statement, int label_offset = 0);
    
    /*
     * Does the same thing as the other rewrite function, but
     * returns the new BoundBlockStatementNode through an "out" parameter
     * and returns the label count after rewriting through the usual means.
     */
    static int rewrite(BoundStatementNode* in_statement, BoundBlockStatementNode*& out_statement, int label_offset = 0);


    static std::vector<BoundGlobalDeclarationNode*> rewrite(std::vector<BoundGlobalDeclarationNode*> bound_program);
};

}

#endif