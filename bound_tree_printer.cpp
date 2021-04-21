#include "bound_tree_printer.h"
#include "binding/tree_rewriter.h"
#include "binding/bound_statement_node.h"
#include "binding/bound_expression_statement_node.h"
#include "binding/bound_error_expression_node.h"
#include "binding/bound_block_statement_node.h"
#include "binding/bound_return_statement_node.h"
#include "binding/bound_empty_statement_node.h"
#include "binding/bound_label_statement_node.h"
#include "binding/bound_if_statement_node.h"
#include "binding/bound_goto_statement_node.h"
#include "binding/bound_for_statement_node.h"
#include "binding/bound_do_while_statement_node.h"
#include "binding/bound_while_statement_node.h"
#include "binding/bound_label_statement_node.h"
#include "binding/bound_goto_statement_node.h"
#include "binding/bound_conditional_goto_statement_node.h"
#include "binding/bound_variable_group_declaration_node.h"
#include "binding/bound_literal_val_expression_node.h"
#include "binding/bound_index_expression_node.h"
#include "binding/bound_struct_declaration_node.h"
#include "binding/bound_variable_reference_expression_node.h"
#include "binding/bound_member_access_expression_node.h"
#include "binding/bound_cast_expression_node.h"
#include "binding/bound_binary_expression_node.h"
#include "binding/bound_unary_expression_node.h"
#include "binding/bound_assignment_expression_node.h"
#include "binding/bound_call_expression_node.h"
#include "binding/bound_ternary_expression_node.h"
#include "binding/bound_increment_expression_node.h"
#include "binding/bound_global_declaration_node.h"
#include "binding/bound_global_statement_node.h"
#include "binding/bound_function_definition_node.h"

#include <string>
#include <iostream>

void BoundTreePrinter::print_bound_tree(std::vector<Binding::BoundGlobalDeclarationNode*> decs) {
    int label_counter = 0;
    for (auto dec : decs) {
        auto kind = dec->kind();
        switch (kind) {
            case Binding::BoundNodeKind::GlobalStatement:
                std::cout << "todo: once I figure out how I want to handle rewriting these" << std::endl;
                break;
            case Binding::BoundNodeKind::FunctionDefinition:
            {
                auto func = dynamic_cast<Binding::BoundFunctionDefinitionNode*>(dec);
                Binding::BoundBlockStatementNode* rewritten = nullptr;
                label_counter = Binding::TreeRewriter::rewrite(func->statements(), rewritten, label_counter);
                print_statement(rewritten);
                break;
            }
            default:
                std::cerr << "what the fuck" << std::endl;
        }
    }
}

void BoundTreePrinter::print_statement( __attribute__((unused)) Binding::BoundStatementNode* statement) {
    std::cout << "yo" << std::endl;
}

void BoundTreePrinter::print_expression_statement( __attribute__((unused)) Binding::BoundExpressionStatementNode* expression_statement) {

}

void BoundTreePrinter::print_block_statement( __attribute__((unused)) Binding::BoundBlockStatementNode* block_statement) {

}

void BoundTreePrinter::print_variable_group_declaration( __attribute__((unused)) Binding::BoundVariableGroupDeclarationNode* var_group_decl) {

}

void BoundTreePrinter::print_variable_declaration( __attribute__((unused)) Binding::BoundVariableDeclarationNode* var_decl) {

}

void BoundTreePrinter::print_if_statement( __attribute__((unused)) Binding::BoundIfStatementNode* if_statement) {

}

void BoundTreePrinter::print_for_statement( __attribute__((unused)) Binding::BoundForStatementNode* for_statement) {

}

void BoundTreePrinter::print_while_statement( __attribute__((unused)) Binding::BoundWhileStatementNode* while_statement) {

}

void BoundTreePrinter::print_do_while_statement( __attribute__((unused)) Binding::BoundDoWhileStatementNode* do_while_statement) {

}

void BoundTreePrinter::print_return_statement( __attribute__((unused)) Binding::BoundReturnStatementNode* return_statement) {

}

void BoundTreePrinter::print_struct_declaration( __attribute__((unused)) Binding::BoundStructDeclarationNode* struct_declaration) {

}

void BoundTreePrinter::print_label_statement( __attribute__((unused)) Binding::BoundLabelStatementNode* label_statement) {

}

void BoundTreePrinter::print_goto_statement( __attribute__((unused)) Binding::BoundGotoStatementNode* goto_statement) {

}

void BoundTreePrinter::print_conditional_goto_statement( __attribute__((unused)) Binding::BoundConditionalGotoStatementNode* conditional_goto_statement) {

}



void BoundTreePrinter::print_expression( __attribute__((unused)) Binding::BoundExpressionNode* expression) {

}

void BoundTreePrinter::print_assignment_expression( __attribute__((unused)) Binding::BoundAssignmentExpressionNode* assignment_expression) {

}

void BoundTreePrinter::print_binary_expression( __attribute__((unused)) Binding::BoundBinaryExpressionNode* binary_expression) {

}

void BoundTreePrinter::print_call_expression( __attribute__((unused)) Binding::BoundCallExpressionNode* call_expression) {

}

void BoundTreePrinter::print_cast_expression( __attribute__((unused)) Binding::BoundCastExpressionNode* cast_expression) {

}

void BoundTreePrinter::print_decrement_expression( __attribute__((unused)) Binding::BoundDecrementExpressionNode* decrement_expression) {

}

void BoundTreePrinter::print_increment_expression( __attribute__((unused)) Binding::BoundIncrementExpressionNode* increment_expression) {

}

void BoundTreePrinter::print_index_expression( __attribute__((unused)) Binding::BoundIndexExpressionNode* index_expression) {

}

void BoundTreePrinter::print_literal_val_expression( __attribute__((unused)) Binding::BoundLiteralValExpressionNode* literal_val_expression) {

}

void BoundTreePrinter::print_member_expression( __attribute__((unused)) Binding::BoundMemberAccessExpressionNode* member_expression) {

}

void BoundTreePrinter::print_var_reference_expression( __attribute__((unused)) Binding::BoundVariableReferenceExpressionNode* var_reference_expression) {

}

void BoundTreePrinter::print_ternary_expression( __attribute__((unused)) Binding::BoundTernaryExpressionNode* ternary_expression) {

}

void BoundTreePrinter::print_unary_expression( __attribute__((unused)) Binding::BoundUnaryExpressionNode* unary_expression) {

}
