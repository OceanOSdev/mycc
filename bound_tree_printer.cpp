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
#include "symbols/variable_symbol.h"
#include "symbols/type_symbol.h"
#include "symbols/function_symbol.h"

#include <iostream>

std::string const BoundTreePrinter::m_indent_str = "  ";

BoundTreePrinter::BoundTreePrinter() : m_indent(0) {}

void BoundTreePrinter::print_indent() const {
    // probably a better and more C++ish way
    // to do this, but I'm only using this for
    // debugging so it's not really mission
    // critical.
    for (int i = 0; i < m_indent; i ++) {
        std::cout << BoundTreePrinter::m_indent_str;
    }
}

void BoundTreePrinter::print_bound_tree(std::vector<Binding::BoundGlobalDeclarationNode*> decs) {
    int label_counter = 0;
    for (auto dec : decs) {
        auto kind = dec->kind();
        switch (kind) {
            case Binding::BoundNodeKind::GlobalStatement:
            {
                auto gdec = dynamic_cast<Binding::BoundGlobalStatementNode*>(dec);
                // forgo from rewriting nonfunction global statements for now
                // since (I think (and I haven't thought much about this)) they shouldn't
                // end up using any conditional statements in them. (i.e. they should be compile time constants)
                print_statement(gdec->statement());
                break;
            }
            case Binding::BoundNodeKind::FunctionDefinition:
            {
                auto func = dynamic_cast<Binding::BoundFunctionDefinitionNode*>(dec);
                Binding::BoundBlockStatementNode* rewritten = nullptr;
                label_counter = Binding::TreeRewriter::rewrite(func->statements(), rewritten, label_counter);
                std::cout << func->function_symbol()->type()->str() << " " << func->function_symbol()->name() << std::endl;
                print_statement(rewritten);
                break;
            }
            default:
                std::cerr << "what the fuck" << std::endl;
        }
    }
}

void BoundTreePrinter::print_statement(Binding::BoundStatementNode* statement) {
    auto kind = statement->kind();
    print_indent();
    switch (kind) {
        case Binding::BoundNodeKind::ExpressionStatement:
            print_expression_statement(dynamic_cast<Binding::BoundExpressionStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::BlockStatement:
            print_block_statement(dynamic_cast<Binding::BoundBlockStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::VariableGroupDeclaration:
            print_variable_group_declaration(dynamic_cast<Binding::BoundVariableGroupDeclarationNode*>(statement));
            break;
        case Binding::BoundNodeKind::VariableDeclaration:
            print_variable_declaration(dynamic_cast<Binding::BoundVariableDeclarationNode*>(statement));
            break;
        case Binding::BoundNodeKind::IfStatement:
            print_if_statement(dynamic_cast<Binding::BoundIfStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::ForStatement:
            print_for_statement(dynamic_cast<Binding::BoundForStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::WhileStatement:
            print_while_statement(dynamic_cast<Binding::BoundWhileStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::DoWhileStatement:
            print_do_while_statement(dynamic_cast<Binding::BoundDoWhileStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::ReturnStatement:
            print_return_statement(dynamic_cast<Binding::BoundReturnStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::StructDeclaration:
            print_struct_declaration(dynamic_cast<Binding::BoundStructDeclarationNode*>(statement));
            break;
        case Binding::BoundNodeKind::LabelStatement:
            print_label_statement(dynamic_cast<Binding::BoundLabelStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::GotoStatement:
            print_goto_statement(dynamic_cast<Binding::BoundGotoStatementNode*>(statement));
            break;
        case Binding::BoundNodeKind::ConditionalGotoStatement:
            print_conditional_goto_statement(dynamic_cast<Binding::BoundConditionalGotoStatementNode*>(statement));
            break;
        default:
            std::cerr << "Unexpected bound node while printing statement." << std::endl;
    }
    std::cout << std::endl;
}

void BoundTreePrinter::print_expression_statement(Binding::BoundExpressionStatementNode* expression_statement) {
    print_expression(expression_statement->expression());
}

void BoundTreePrinter::print_block_statement(Binding::BoundBlockStatementNode* block_statement) {
    std::cout << '{' << std::endl;
    m_indent++;
    for (auto statement : block_statement->statements()) {
        print_statement(statement);
    }
    m_indent--;
    std::cout << '}';
}

void BoundTreePrinter::print_variable_group_declaration(Binding::BoundVariableGroupDeclarationNode* var_group_decl) {
    std::vector<Binding::BoundVariableDeclarationNode*>::iterator var_decl_iter;
    auto var_decs = var_group_decl->variable_declarations();
    auto last = std::prev(var_decs.end(), 1);
    for(var_decl_iter = var_decs.begin(); var_decl_iter != var_decs.end(); var_decl_iter++ ) {
        print_variable_declaration(*var_decl_iter);
        if (var_decl_iter != last) {
            std::cout << std::endl;
            print_indent();
        }
    }
}

void BoundTreePrinter::print_variable_declaration(Binding::BoundVariableDeclarationNode* var_decl) {
    auto var = var_decl->variable_symbol();
    std::cout << var->var_type()->str() << " " 
              << (var->is_array() ? "(size: " + std::to_string(var->array_size()) + ") " : "")
              << var->name();
            
    if (var_decl->is_initialized()) {
        std::cout << " = ";
        print_expression(var_decl->initializer());
    }
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


void BoundTreePrinter::print_expression(Binding::BoundExpressionNode* expression) {
    auto kind = expression->kind();
    switch (kind) {
        case Binding::BoundNodeKind::AssignmentExpression:
            print_assignment_expression(dynamic_cast<Binding::BoundAssignmentExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::BinaryExpression:
            print_binary_expression(dynamic_cast<Binding::BoundBinaryExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::CallExpression:
            print_call_expression(dynamic_cast<Binding::BoundCallExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::CastExpression:
            print_cast_expression(dynamic_cast<Binding::BoundCastExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::DecrementExpression:
            print_decrement_expression(dynamic_cast<Binding::BoundDecrementExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::IncrementExpression:
            print_increment_expression(dynamic_cast<Binding::BoundIncrementExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::IndexExpression:
            print_index_expression(dynamic_cast<Binding::BoundIndexExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::LiteralExpression:
            print_literal_val_expression(dynamic_cast<Binding::BoundLiteralValExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::MemberAccessExpression:
            print_member_expression(dynamic_cast<Binding::BoundMemberAccessExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::VariableReferenceExpression:
            print_var_reference_expression(dynamic_cast<Binding::BoundVariableReferenceExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::TernaryExpression:
            print_ternary_expression(dynamic_cast<Binding::BoundTernaryExpressionNode*>(expression));
            break;
        case Binding::BoundNodeKind::UnaryExpression:
            print_unary_expression(dynamic_cast<Binding::BoundUnaryExpressionNode*>(expression));
            break;
        default:
            std::cerr << "Unexpected bound node while rewriting expression." << std::endl;
    }
}

void BoundTreePrinter::print_assignment_expression(Binding::BoundAssignmentExpressionNode* assignment_expression) {
    print_expression(assignment_expression->variable_reference());
    std::cout << " ";
    print_assignment_type(assignment_expression->assignment_op_kind());
    std::cout << " ";
    print_expression(assignment_expression->expression());
}

void BoundTreePrinter::print_binary_expression( __attribute__((unused)) Binding::BoundBinaryExpressionNode* binary_expression) {
    print_expression(binary_expression->left());
    std::cout << " ";
    print_binary_op_token(binary_expression->op()->op_kind());
    std::cout << " ";
    print_expression(binary_expression->right());
}

void BoundTreePrinter::print_call_expression( __attribute__((unused)) Binding::BoundCallExpressionNode* call_expression) {

}

void BoundTreePrinter::print_cast_expression(Binding::BoundCastExpressionNode* cast_expression) {
    std::cout << "(" << cast_expression->type()->str() << ")(";
    print_expression(cast_expression->expression());
    std::cout << ")";
}

void BoundTreePrinter::print_decrement_expression( __attribute__((unused)) Binding::BoundDecrementExpressionNode* decrement_expression) {

}

void BoundTreePrinter::print_increment_expression( __attribute__((unused)) Binding::BoundIncrementExpressionNode* increment_expression) {

}

void BoundTreePrinter::print_index_expression(Binding::BoundIndexExpressionNode* index_expression) {
    std::cout << index_expression->variable_reference()->name() << '[';
    print_expression(index_expression->expression());
    std::cout << ']';
}

void BoundTreePrinter::print_literal_val_expression(Binding::BoundLiteralValExpressionNode* literal_val_expression) {
    auto lit_type = literal_val_expression->type();
    std::string lit_type_id = lit_type->name();
    if (lit_type_id == "int") {
        std::cout << literal_val_expression->int_value();
    } else if (lit_type_id == "float") {
        std::cout << literal_val_expression->float_value();
    } else if (lit_type_id == "char") {
        if (!lit_type->attributes().is_array)
            std::cout << "'" << literal_val_expression->char_value() << "'";
        else
            std::cout << "\"" << literal_val_expression->string_value() << "\"";
    }
}

void BoundTreePrinter::print_member_expression( __attribute__((unused)) Binding::BoundMemberAccessExpressionNode* member_expression) {

}

void BoundTreePrinter::print_var_reference_expression(Binding::BoundVariableReferenceExpressionNode* var_reference_expression) {
    std::cout << var_reference_expression->variable_reference()->name();
}

void BoundTreePrinter::print_ternary_expression( __attribute__((unused)) Binding::BoundTernaryExpressionNode* ternary_expression) {
    print_expression(ternary_expression->conditional());
    std::cout << " ? ";
    print_expression(ternary_expression->true_expr());
    std::cout << " : ";
    print_expression(ternary_expression->false_expr());
}

void BoundTreePrinter::print_unary_expression(Binding::BoundUnaryExpressionNode* unary_expression) {
    print_unary_op_token(unary_expression->op()->op_kind());
    std::cout << " ";
    print_expression(unary_expression->expression());
}


void BoundTreePrinter::print_assignment_type(Binding::BoundAssignmentOpKind assignment_token) {
    std::string token = "";
    switch(assignment_token) {
        case Binding::BoundAssignmentOpKind::ASSIGN: token = "="; break;
        case Binding::BoundAssignmentOpKind::PLUS_ASSIGN: token = "+="; break;
        case Binding::BoundAssignmentOpKind::MINUS_ASSIGN: token = "-="; break;
        case Binding::BoundAssignmentOpKind::STAR_ASSIGN: token = "*="; break;
        case Binding::BoundAssignmentOpKind::SLASH_ASSIGN: token = "/="; break;
    }

    std::cout << token;
}

void BoundTreePrinter::print_binary_op_token(Binding::BoundBinaryOpKind binary_op_token) {
    std::string token = "";
    switch (binary_op_token) {
        case Binding::BoundBinaryOpKind::Modulo: token = "%"; break;
        case Binding::BoundBinaryOpKind::BitwiseAnd: token = "&"; break;
        case Binding::BoundBinaryOpKind::BitwiseOr: token = "|"; break;
        case Binding::BoundBinaryOpKind::Addition: token = "+"; break;
        case Binding::BoundBinaryOpKind::Subtraction: token = "-"; break;
        case Binding::BoundBinaryOpKind::Multiplication: token = "*"; break;
        case Binding::BoundBinaryOpKind::Division: token = "/"; break;
        case Binding::BoundBinaryOpKind::Equals: token = "=="; break;
        case Binding::BoundBinaryOpKind::NotEquals: token = "!="; break;
        case Binding::BoundBinaryOpKind::LessThan: token = "<"; break;
        case Binding::BoundBinaryOpKind::LessThanOrEquals: token = "<="; break;
        case Binding::BoundBinaryOpKind::GreaterThan: token = ">"; break;
        case Binding::BoundBinaryOpKind::GreaterThanOrEquals: token = ">="; break;
        case Binding::BoundBinaryOpKind::LogicalAnd: token = "&&"; break;
        case Binding::BoundBinaryOpKind::LogicalOr: token = "||"; break;
    }
    std::cout << token;
}

void BoundTreePrinter::print_unary_op_token(Binding::BoundUnaryOpKind unary_op_token) {
    char token = 0;
    switch (unary_op_token) {
        case Binding::BoundUnaryOpKind::OnesComplement: token = '~'; break;
        case Binding::BoundUnaryOpKind::Negation: token = '-'; break;
        case Binding::BoundUnaryOpKind::LogicalNegation: token = '!'; break;
    }

    std::cout << token;
}
