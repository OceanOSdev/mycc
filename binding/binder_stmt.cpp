/* LIB INCLUDES */
#include <stdexcept>

/* HEADER INCLUDE */
#include "binder.h"

/* BOUND TREE NODE INCLUDES */
#include "bound_statement_node.h"
#include "bound_expression_statement_node.h"
#include "bound_error_expression_node.h"
#include "bound_scope.h"
#include "bound_block_statement_node.h"
#include "bound_return_statement_node.h"
#include "bound_empty_statement_node.h"
#include "bound_label_statement_node.h"
#include "bound_if_statement_node.h"
#include "bound_goto_statement_node.h"
#include "bound_for_statement_node.h"
#include "bound_do_while_statement_node.h"
#include "bound_while_statement_node.h"
#include "bound_variable_group_declaration_node.h"
#include "bound_struct_declaration_node.h"


/* UTILITY INCLUDES */
#include "../logging/diagnostics.h"
#include "../logging/part_three_info.h"

/* SYMBOL INCLUDES */
#include "../symbols/type_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/function_symbol.h"
#include "../symbols/variable_symbol.h"
#include "../symbols/struct_symbol.h"

/* SYNTAX TREE NODE INCLUDES */
#include "../syntax/statement_node.h"
#include "../syntax/syntax_kind.h"
#include "../syntax/syntax_token.h"
#include "../syntax/expression_statement_node.h"
#include "../syntax/variable_group_declaration_node.h"
#include "../syntax/struct_declaration_node.h"
#include "../syntax/block_statement_node.h"
#include "../syntax/return_statement_node.h"
#include "../syntax/break_statement_node.h"
#include "../syntax/continue_statement_node.h"
#include "../syntax/if_statement_node.h"
#include "../syntax/for_statement_node.h"
#include "../syntax/do_while_statement_node.h"
#include "../syntax/while_statement_node.h"

#define BREAK_LABEL 0
#define CONTINUE_LABEL 1

namespace Binding {

BoundStatementNode* Binder::bind_error_statement() {
    return new BoundExpressionStatementNode(bind_error_expression());
}

BoundStatementNode* Binder::bind_statement(Syntax::StatementNode* statement) {
    if (m_err_flag) return bind_error_statement();
    auto syntaxKind = statement->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::BlockStatement: 
            return bind_block_statement(dynamic_cast<Syntax::BlockStatementNode*>(statement));
        case Syntax::SyntaxKind::BreakStatement: 
            return bind_break_statement(dynamic_cast<Syntax::BreakStatementNode*>(statement));
        case Syntax::SyntaxKind::ContinueStatement: 
            return bind_continue_statement(dynamic_cast<Syntax::ContinueStatementNode*>(statement));
        case Syntax::SyntaxKind::DoWhileStatement: 
            return bind_do_while_statement(dynamic_cast<Syntax::DoWhileStatementNode*>(statement));
        case Syntax::SyntaxKind::ExpressionStatement:
            return bind_expression_statement(dynamic_cast<Syntax::ExpressionStatementNode*>(statement)); 
        case Syntax::SyntaxKind::ForStatement: 
            return bind_for_statement(dynamic_cast<Syntax::ForStatementNode*>(statement));
        case Syntax::SyntaxKind::StructDeclaration: 
            return bind_struct_declaration(dynamic_cast<Syntax::StructDeclarationNode*>(statement));
        case Syntax::SyntaxKind::VariableDeclaration: 
            return bind_variable_group_declaration(
                dynamic_cast<Syntax::VariableGroupDeclarationNode*>(statement)
            );
        case Syntax::SyntaxKind::IfStatement: 
            return bind_if_statement(dynamic_cast<Syntax::IfStatementNode*>(statement));
        case Syntax::SyntaxKind::ReturnStatement: 
            return bind_return_statement(dynamic_cast<Syntax::ReturnStatementNode*>(statement));
        case Syntax::SyntaxKind::WhileStatement: 
            return bind_while_statement(dynamic_cast<Syntax::WhileStatementNode*>(statement));
        default:
            throw std::runtime_error("Unexpected syntax while binding statement.");
    }

    return nullptr;
}

BoundStatementNode* Binder::bind_block_statement(Syntax::BlockStatementNode* block_statement, bool create_new_scope) {
    if (create_new_scope) {
        m_scope = new BoundScope(m_scope); //maybe have the parent create new scope and pass it in?
    }
    std::vector<BoundStatementNode*> bound_statements;
    for (auto statement : block_statement->statements()) {
        auto bound_statement = bind_statement(statement);
        bound_statements.push_back(bound_statement);
    }

    // careful, we might have to make leaving scope the responsibility
    // of the parent (I don't think we'll have to, but in case of scope bugs, check here first)
    m_scope = m_scope->parent();
    return new BoundBlockStatementNode(bound_statements);
}

BoundStatementNode* Binder::bind_break_statement(Syntax::BreakStatementNode* break_statement) {
    if (m_loop_stack.empty()) {
        m_diagnostics->report_invalid_break_or_continue(break_statement->token(), "break");
        return bind_error_statement();
    }

    auto label = std::get<BREAK_LABEL>(m_loop_stack.top());
    return new BoundGotoStatementNode(label);
}

BoundStatementNode* Binder::bind_continue_statement(Syntax::ContinueStatementNode* continue_statement) {
    if (m_loop_stack.empty()) {
        m_diagnostics->report_invalid_break_or_continue(continue_statement->token(), "continue");
        return bind_error_statement();
    }

    auto label = std::get<CONTINUE_LABEL>(m_loop_stack.top());
    return new BoundGotoStatementNode(label);
}

BoundStatementNode* Binder::bind_do_while_statement(Syntax::DoWhileStatementNode* do_while_statement) {
    BoundExpressionNode* bound_condition_expression = bind_expression(do_while_statement->conditional_expression());
    if (Symbols::TypeSymbol::is_error_type(bound_condition_expression->type()))
        return bind_error_statement();
    
    if (!Symbols::TypeSymbol::is_conditional_type(bound_condition_expression->type())) {
        m_diagnostics->report_invalid_condition_do_while(do_while_statement->token(), bound_condition_expression->type()->str());
        return bind_error_statement();
    }

    BoundLabel* break_lbl;
    BoundLabel* continue_lbl;
    auto bound_body = bind_loop_body(do_while_statement->body_statement(), break_lbl, continue_lbl);
    return new BoundDoWhileStatementNode(bound_condition_expression, bound_body, break_lbl, continue_lbl);
}

BoundStatementNode* Binder::bind_expression_statement(Syntax::ExpressionStatementNode* expression_statement) {
    if (expression_statement->expression() == nullptr) return new BoundEmptyStatementNode();
    auto expression = bind_expression(expression_statement->expression(), true);
    m_part_three_info_list->func_builder_add_statement(expression_statement->token()->begin_line(), expression->type()->str());
    return new BoundExpressionStatementNode(expression);
}

BoundStatementNode* Binder::bind_for_statement(Syntax::ForStatementNode* for_statement) {
    BoundExpressionNode* bound_initial_expression = nullptr;
    BoundExpressionNode* bound_condition_expression = nullptr;
    BoundExpressionNode* bound_third_expression = nullptr;

    bool err_type_flag = false;

    if (for_statement->has_initial_expression()) {
        bound_initial_expression = bind_expression(for_statement->initial_expression());
        err_type_flag |= Symbols::TypeSymbol::is_error_type(bound_initial_expression->type());
    }
    
    if (for_statement->has_conditional_expression()) {
        bound_condition_expression = bind_expression(for_statement->conditional_expression());
        err_type_flag |= Symbols::TypeSymbol::is_error_type(bound_condition_expression->type());
    }
    
    if (for_statement->has_third_expression()) {
        bound_third_expression = bind_expression(for_statement->third_expression());
        err_type_flag |= Symbols::TypeSymbol::is_error_type(bound_third_expression->type());
    }
    
    if (bound_condition_expression != nullptr && !Symbols::TypeSymbol::is_conditional_type(bound_condition_expression->type())) {
        m_diagnostics->report_invalid_condition_for(for_statement->token(), bound_condition_expression->type()->str());
        err_type_flag = true;
    }

    if (err_type_flag)
        return bind_error_statement();

    BoundLabel* break_lbl;
    BoundLabel* continue_lbl;
    auto bound_body = bind_loop_body(for_statement->body_statement(), break_lbl, continue_lbl);
    return new BoundForStatementNode(bound_initial_expression, bound_condition_expression, bound_third_expression, bound_body, break_lbl, continue_lbl);
}


BoundStatementNode* Binder::bind_if_statement(Syntax::IfStatementNode* if_statement) {
    auto bound_condition = bind_expression(if_statement->condition());
    if (!Symbols::TypeSymbol::is_conditional_type(bound_condition->type())) {
        m_diagnostics->report_invalid_condition_if(if_statement->token(), bound_condition->type()->str());
        return bind_error_statement();
    }

    auto bound_statement = bind_statement(if_statement->then_statement());
    auto bound_else_stmt = if_statement->has_else_statement() ? bind_statement(if_statement->else_statement()) : nullptr;

    return new BoundIfStatementNode(bound_condition, bound_statement, bound_else_stmt);
}

BoundStatementNode* Binder::bind_return_statement(Syntax::ReturnStatementNode* return_statement) {
    auto bound_expression = return_statement->is_empty_return() ? nullptr : bind_expression(return_statement->expression(), true);

    auto f_return_type = m_current_function->type();

    if (Symbols::TypeSymbol::are_types_equivalent(f_return_type, &Symbols::TypeSymbol::Void)) {
        if (bound_expression != nullptr && 
            !Symbols::TypeSymbol::are_types_equivalent(bound_expression->type(),&Symbols::TypeSymbol::Void)) {
                std::string expr_ret_type = bound_expression->type()->str();
                m_diagnostics->report_invalid_return_type(return_statement->token(), expr_ret_type, Symbols::TypeSymbol::Void.str());
                m_err_flag = true;
        }
    } else  {
        if (bound_expression == nullptr) {
            m_diagnostics->report_invalid_return_type(return_statement->token(), Symbols::TypeSymbol::Void.str(), f_return_type->str());
            m_err_flag = true;
        } else if (!Symbols::TypeSymbol::are_types_equivalent(bound_expression->type(), f_return_type)) {
            m_diagnostics->report_invalid_return_type(return_statement->token(), bound_expression->type()->str(), f_return_type->str());
            m_err_flag = true;
        }
    }

    return new BoundReturnStatementNode(bound_expression);
}

BoundStatementNode* Binder::bind_while_statement(Syntax::WhileStatementNode* while_statement) {
    BoundExpressionNode* bound_condition_expression = bind_expression(while_statement->conditional_expression());
    if (Symbols::TypeSymbol::is_error_type(bound_condition_expression->type()))
        return bind_error_statement();
    
    if (!Symbols::TypeSymbol::is_conditional_type(bound_condition_expression->type())) {
        m_diagnostics->report_invalid_condition_while(while_statement->token(), bound_condition_expression->type()->str());
        return bind_error_statement();
    }

    BoundLabel* break_lbl;
    BoundLabel* continue_lbl;
    auto bound_body = bind_loop_body(while_statement->body_statement(), break_lbl, continue_lbl);
    return new BoundWhileStatementNode(bound_condition_expression, bound_body, break_lbl, continue_lbl);
}

BoundStatementNode* Binder::bind_loop_body(Syntax::StatementNode* body, BoundLabel*& break_label, BoundLabel*& continue_label) {
    m_label_idx++;
    std::string label_idx_str = std::to_string(m_label_idx);
    // label text is prefixed with "b_" to signal that the label was
    // generated by the binder. The only purpose that serves at the 
    // moment is to potentially help me debug during codegen.
    std::string break_label_text = "b_break_label_" + label_idx_str;
    std::string continue_label_text = "b_continue_label_" + label_idx_str;
    break_label = new BoundLabel(break_label_text);
    continue_label = new BoundLabel(continue_label_text);
    
    m_loop_stack.push(std::make_tuple(break_label, continue_label));
    auto bound_statement = bind_statement(body);
    m_loop_stack.pop();

    return bound_statement;
}

BoundStatementNode* Binder::bind_struct_declaration(Syntax::StructDeclarationNode* struct_declaration) {
    std::string identifier = struct_declaration->struct_name();
    m_part_three_info_list->struct_builder_add_name(identifier);
    // create new scope so that struct member names don't have conflicts with
    // current scope variable names
    m_scope = new BoundScope(m_scope);
    std::vector<BoundVariableGroupDeclarationNode*> bound_member_groups;
    for (auto var_group : struct_declaration->struct_members()) {
        m_part_three_info_list->ignore = true;
        auto bound_group = bind_statement(var_group);
        m_part_three_info_list->ignore = false;
        bound_member_groups.push_back(dynamic_cast<BoundVariableGroupDeclarationNode*>(bound_group));
    }

    m_scope = m_scope->parent();
    
    std::vector<Symbols::VariableSymbol*> bound_members;
    for (auto member_group : bound_member_groups) {
        for (auto member_decl : member_group->variable_declarations()) {
            m_part_three_info_list->struct_builder_add_member(member_decl->variable_symbol()->var_type()->str(), member_decl->variable_symbol()->name());
            bound_members.push_back(member_decl->variable_symbol());
        }
    }

    Symbols::StructSymbol* struct_symbol = new Symbols::StructSymbol(identifier, bound_members);
    const Symbols::TypeSymbol* struct_type_symbol = new Symbols::TypeSymbol(identifier, {true});
    bool decl_results = m_scope->try_declare_type(struct_type_symbol);
    decl_results &= m_scope->try_declare_struct(struct_symbol);
    if (!decl_results) {
        m_diagnostics->report_struct_already_defined(struct_declaration->token(), identifier);
        m_err_flag = true;
        m_part_three_info_list->struct_builder_build(true, true);
        return bind_error_statement();
    }
    
    m_part_three_info_list->struct_builder_build(m_current_function == nullptr);
    return new BoundStructDeclarationNode(struct_symbol);
}

BoundStatementNode* Binder::bind_variable_group_declaration(Syntax::VariableGroupDeclarationNode* variable_group) {
    std::vector<BoundVariableDeclarationNode*> bound_variables;
    auto type_symbol_id = variable_group->type();
    auto var_type = bind_type_clause(type_symbol_id);
    if (Symbols::TypeSymbol::is_error_or_incomplete_type(var_type)) {
        m_diagnostics->report_no_definition_for_type_symbol(variable_group->token(), var_type->str());
        m_err_flag = true;
    }

    for (auto partial_dec : variable_group->partial_variable_group()) {
        std::string ident = partial_dec->identifier();
        BoundExpressionNode* initial_val = nullptr;

        bool is_array = partial_dec->is_array();
        bool is_const = variable_group->is_const();
        const Symbols::TypeSymbol* type = var_type;
        if (is_array)
            type = type->as_array_type();
        if (is_const)
            type = type->as_const_type();
        
        auto variable_symbol = new Symbols::VariableSymbol(ident, 
                                                            type,
                                                            is_array,
                                                            partial_dec->array_length(),
                                                            is_const);
        
        if (partial_dec->is_assigned()) {
            initial_val = bind_expression(partial_dec->expression());
            if (!Symbols::TypeSymbol::are_types_equivalent(initial_val->type(), type)) {
                m_diagnostics->report_incompatible_conversion(partial_dec->token(), initial_val->type()->str(), type->str());
                m_err_flag = true;
                initial_val = new BoundErrorExpressionNode();
            }
        }
        
        auto added_to_scope = m_scope->try_declare_variable(variable_symbol);
        if (!added_to_scope) {
            m_diagnostics->report_variable_already_declared(partial_dec->token(), variable_symbol->name(), m_current_function == nullptr);
            m_err_flag = true;
        }

        m_part_three_info_list->add_var_info(variable_symbol->var_type()->str(), ident, partial_dec->is_assigned(), m_current_function == nullptr);

        bound_variables.push_back(new BoundVariableDeclarationNode(variable_symbol, initial_val));
    }

    return new BoundVariableGroupDeclarationNode(bound_variables);
}

}