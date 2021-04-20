/* LIB INCLUDES */
#include <stdexcept>
#include <set>
#include <algorithm>

/* HEADER INCLUDE */
#include "binder.h"

/* BOUND TREE NODE INCLUDES */
#include "bound_statement_node.h"
#include "bound_expression_statement_node.h"
#include "bound_error_expression_node.h"
#include "bound_scope.h"
#include "bound_global_declaration_node.h"
#include "bound_global_statement_node.h"
#include "bound_function_definition_node.h"
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

/* UTILITY INCLUDES */
#include "../logging/diagnostics.h"
#include "../logging/part_three_info.h"

/* SYMBOL INCLUDES */
#include "../symbols/type_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/function_symbol.h"
#include "../symbols/variable_symbol.h"
#include "../symbols/struct_symbol.h"
#include "../symbols/parameter_symbol.h"

/* SYNTAX TREE NODE INCLUDES */
#include "../syntax/statement_node.h"
#include "../syntax/syntax_kind.h"
#include "../syntax/cast_expression_node.h"
#include "../syntax/statement_node.h"
#include "../syntax/expression_node.h"
#include "../syntax/expression_statement_node.h"
#include "../syntax/global_declaration_node.h"
#include "../syntax/global_variable_group_declaration_node.h"
#include "../syntax/global_struct_declaration_node.h"
#include "../syntax/function_prototype_node.h"
#include "../syntax/function_definition_node.h"
#include "../syntax/formal_parameter_node.h"
#include "../syntax/block_statement_node.h"
#include "../syntax/return_statement_node.h"
#include "../syntax/break_statement_node.h"
#include "../syntax/continue_statement_node.h"
#include "../syntax/if_statement_node.h"
#include "../syntax/for_statement_node.h"
#include "../syntax/do_while_statement_node.h"
#include "../syntax/while_statement_node.h"
#include "../syntax/variable_group_declaration_node.h"
#include "../syntax/partial_variable_declaration_node.h"
#include "../syntax/literal_val_expression_node.h"
#include "../syntax/index_expression_node.h"
#include "../syntax/struct_declaration_node.h"
#include "../syntax/name_expression_node.h"
#include "../syntax/member_expression_node.h"
#include "../syntax/binary_expression_node.h"
#include "../syntax/unary_expression_node.h"
#include "../syntax/ternary_expression_node.h"
#include "../syntax/increment_expression_node.h"
#include "../syntax/decrement_expression_node.h"
#include "../syntax/assignment_expression_node.h"
#include "../syntax/call_expression_node.h"
#include "../syntax/program_node.h"

#define BREAK_LABEL 0
#define CONTINUE_LABEL 1

namespace Binding {

Binder::Binder(Logging::DiagnosticsList* diagnostics, BoundScope* parent) : 
    m_diagnostics(diagnostics),
    m_part_three_info_list(new Logging::PartThreeInfoList()), 
    m_err_flag(false), 
    m_label_idx(0), 
    m_scope(new BoundScope(parent)),
    m_current_function(nullptr) {

}

BoundScope* Binder::init_global_scope() {
    BoundScope* scope = new BoundScope(nullptr);

    // add default types
    scope->try_declare_type(&Symbols::TypeSymbol::Char);
    scope->try_declare_type(&Symbols::TypeSymbol::Int);
    scope->try_declare_type(&Symbols::TypeSymbol::Float);
    scope->try_declare_type(&Symbols::TypeSymbol::Void);
    // Maybe declare error type?
    //scope->try_declare_type(&Symbols::TypeSymbol::Error);

    return scope;
}


/*
 * For right now, return the local binder since I'm not sure
 * what to include in the BoundProgram.
 */
Binder* Binder::bind_program(Syntax::ProgramNode* program) {
    // Create root scope directly since we aren't
    // *actually* supporting multiple input files
    BoundScope* scope = init_global_scope();
    Logging::DiagnosticsList* diag = new Logging::DiagnosticsList();
    auto binder = new Binder(diag, scope);

    // Again, since we only really support one input
    // file, we only need to grab the first translation
    // unit.
    auto translationUnit = program->units()[0];
    for (auto dec : translationUnit->global_declarations()) {
        binder->bind_global_declaration(dec);
        if (binder->err_flag()) break;
    }


    return binder;
}

/*
 * List of top level statements and declarations in the program.
 */
std::vector<BoundGlobalDeclarationNode*> Binder::global_decls() const {
    return m_global_decls;
}

/*
 * List of diagnostics describing any errors that may
 * have occured during binding.
 */
Logging::DiagnosticsList* Binder::diagnostics() const {
    return m_diagnostics;
}

/*
 * Custom struct containing the info to output
 * for part 3.
 */
Logging::PartThreeInfoList* Binder::part_three_info_list() const {
    return m_part_three_info_list;
}

/*
 * Bool flag which returns true if there was an error
 * during binding.
 */
bool Binder::err_flag() const {
    return m_err_flag;
}


void Binder::set_current_function_scope(Symbols::FunctionSymbol* function_symbol) {
    m_current_function = function_symbol;
}


void Binder::bind_global_declaration(Syntax::GlobalDeclarationNode* gdn) {
    if (m_err_flag) return; // bail early.
    auto syntaxKind = gdn->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::GlobalVariableDeclaration:
        {
            auto vardec = 
                (dynamic_cast<Syntax::GlobalVariableGroupDeclarationNode*>(gdn))->variable_group();
            m_global_decls.push_back(new BoundGlobalStatementNode(bind_statement(vardec)));
            break;
        }
        case Syntax::SyntaxKind::GlobalStructDeclaration:
        {
            auto structdec = 
                (dynamic_cast<Syntax::GlobalStructDeclarationNode*>(gdn))->struct_declaration();
            m_global_decls.push_back(new BoundGlobalStatementNode(bind_statement(structdec)));
            break;
        }
        case Syntax::SyntaxKind::FunctionPrototype:
        {
            auto proto = dynamic_cast<Syntax::FunctionPrototypeNode*>(gdn);
            bind_function_prototype(proto);
            break;
        }
        case Syntax::SyntaxKind::FunctionDefinition:
        {
            auto func_def = dynamic_cast<Syntax::FunctionDefinitionNode*>(gdn);
            m_global_decls.push_back(bind_function_definition(func_def));
            break;
        }
        default:
            throw std::runtime_error("Unexpected syntax while binding global statement.");
    }
}

Symbols::FunctionSymbol* Binder::bind_function_declaration(Syntax::FunctionDeclarationNode* declaration) {
    std::vector<Symbols::ParameterSymbol*> params;
    std::set<std::string> param_name_set; // keep track if we've seen a parameter with a given name
    for (auto parameter_syntax : declaration->parameters()) {
        auto ins_res = param_name_set.insert(parameter_syntax->param_name());
        if (!ins_res.second) {
            m_diagnostics->report_redefinition_of_parameter(parameter_syntax->token(), parameter_syntax->param_name());
            m_err_flag = true;
            return nullptr;
        }
        std::string type_symbol_id = parameter_syntax->type_identifier();
        auto param_type = bind_type_clause(type_symbol_id);
        if (Symbols::TypeSymbol::is_error_type(param_type)) {
            if (parameter_syntax->is_struct()) type_symbol_id = "struct " + type_symbol_id; // probably not the best way to accomplish this.
            m_diagnostics->report_no_definition_for_type_symbol(parameter_syntax->token(), type_symbol_id);
            m_err_flag = true;
            return nullptr;
        }

        const Symbols::TypeSymbol* parameter_type = param_type;
        bool is_array = parameter_syntax->is_array();
        bool is_const = parameter_syntax->is_const();
        if (is_array) parameter_type = parameter_type->as_array_type();
        if (is_const) parameter_type = parameter_type->as_const_type();
        params.push_back(new Symbols::ParameterSymbol(parameter_syntax->param_name(), 
                                                      parameter_type, 
                                                      is_array, 
                                                      is_const));
        
        m_part_three_info_list->func_builder_add_param(params.back()->type()->str(), params.back()->name());
    }

    std::string func_type_id = declaration->type_identifier();
    auto func_type = bind_type_clause(func_type_id);
    if (Symbols::TypeSymbol::is_error_type(func_type)) {
        m_diagnostics->report_no_definition_for_type_symbol(declaration->token(), func_type->str());
        m_err_flag = true;
        return nullptr;
    }

    m_part_three_info_list->func_builder_add_type(func_type->str());
    m_part_three_info_list->func_builder_add_name(declaration->function_name());

    return new Symbols::FunctionSymbol(declaration->function_name(), func_type, params);
}

void Binder::bind_function_prototype(Syntax::FunctionPrototypeNode* prototype) {
    m_part_three_info_list->ignore = true;
    auto symbol = bind_function_declaration(prototype->function_declaration());
    m_part_three_info_list->ignore = false;
    if (!m_err_flag && !m_scope->try_declare_function(symbol)) {
        m_diagnostics->report_conflicting_function_declarations(prototype->token(), symbol->name());
        m_err_flag = true;
    }
}

BoundFunctionDefinitionNode* Binder::bind_function_definition(Syntax::FunctionDefinitionNode* function_definition) {
    auto symbol = bind_function_declaration(function_definition->function_declaration());
    if (m_err_flag) return nullptr;
    if (!m_scope->try_declare_function(symbol)) {
        m_diagnostics->report_conflicting_function_declarations(function_definition->token(), symbol->name());
        m_err_flag = true;
        return nullptr; //Probably want to do something a bit cleaner than this.
    }
    Symbols::FunctionSymbol* func_in_scope = nullptr;
    m_scope->try_look_up_function(symbol->name(), func_in_scope);
    if (func_in_scope == nullptr) {
        // Something terribly wrong happened seeing as just above
        // we inserted it into the symbol table if it didn't already.
        // exist.
        throw std::runtime_error("Function failed to get added to symbol table.");
    }
    if (func_in_scope->line_defined() != -1) {
        m_diagnostics->report_function_already_defined(function_definition->token(), symbol->name());
        m_err_flag = true;
        return nullptr;
    }
    func_in_scope->set_line_defined(function_definition->token()->begin_line());

    // put parameters in new scope
    m_scope = new BoundScope(m_scope);
    for (auto param : symbol->params()) {
        m_scope->try_declare_variable(Symbols::VariableSymbol::from_parameter_symbol(param));
    }
    
    std::vector<Syntax::StatementNode*> body_syntax;
    for (auto declaration : function_definition->local_declarations())
        body_syntax.push_back(declaration);
    for (auto stmt : function_definition->statements())
        body_syntax.push_back(stmt);
    
    auto body_block_syntax = new Syntax::BlockStatementNode(nullptr, body_syntax);
    set_current_function_scope(symbol);
    auto bound_body = bind_block_statement(body_block_syntax,false);
    set_current_function_scope(nullptr);
    m_part_three_info_list->func_builder_build();
    return new BoundFunctionDefinitionNode(symbol, dynamic_cast<BoundBlockStatementNode*>(bound_body));
}

BoundStatementNode* Binder::bind_error_statement() {
    return new BoundExpressionStatementNode(bind_error_expression());
}

BoundExpressionNode* Binder::bind_error_expression() {
    m_err_flag = true;
    return new BoundErrorExpressionNode();
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
    if (Symbols::TypeSymbol::is_error_type(var_type)) {
        m_diagnostics->report_no_definition_for_type_symbol(variable_group->token(), var_type->str());
        m_err_flag = true;
    }

    for (auto partial_dec : variable_group->partial_variable_group()) {
        std::string ident = partial_dec->identifier();
        BoundExpressionNode* initial_val = nullptr;
        if (partial_dec->is_assigned()) {
            initial_val = bind_expression(partial_dec->expression());
            if (!Symbols::TypeSymbol::are_types_equivalent(initial_val->type(), var_type)) {
                m_diagnostics->report_incompatible_conversion(partial_dec->token(), initial_val->type()->str(), var_type->str());
                m_err_flag = true;
                initial_val = new BoundErrorExpressionNode();
            }
        }

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


BoundExpressionNode* Binder::bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid) {
    if (m_err_flag) return new BoundErrorExpressionNode();
    auto result = bind_expression_internal(expression);
    if (Symbols::TypeSymbol::are_types_equivalent(result->type(), &Symbols::TypeSymbol::Error))
        return new BoundErrorExpressionNode(); // m_err_flag should already be set, in case of bugs though, take a gander here.
    if (!canBeVoid && Symbols::TypeSymbol::are_types_equivalent(result->type(), &Symbols::TypeSymbol::Void)) {
        m_diagnostics->report_expression_cannot_be_void(expression->token());
        return bind_error_expression();
    }
    return result;
}


BoundExpressionNode* Binder::bind_expression_internal(Syntax::ExpressionNode* expression) {
    auto syntaxKind = expression->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::AssignmentExpression:
            return bind_assignment_expression(dynamic_cast<Syntax::AssignmentExpressionNode*>(expression));
        case Syntax::SyntaxKind::BinaryExpression:
            return bind_binary_expression(dynamic_cast<Syntax::BinaryExpressionNode*>(expression));
        case Syntax::SyntaxKind::CallExpression:
            return bind_call_expression(dynamic_cast<Syntax::CallExpressionNode*>(expression));
        case Syntax::SyntaxKind::CastExpression:
            return bind_cast_expression(dynamic_cast<Syntax::CastExpressionNode*>(expression));
        case Syntax::SyntaxKind::DecrementExpression:
            return bind_decrement_expression(dynamic_cast<Syntax::DecrementExpressionNode*>(expression));
        case Syntax::SyntaxKind::IncrementExpression:
            return bind_increment_expression(dynamic_cast<Syntax::IncrementExpressionNode*>(expression));
        case Syntax::SyntaxKind::IndexExpression:
            return bind_index_expression(dynamic_cast<Syntax::IndexExpressionNode*>(expression));
        case Syntax::SyntaxKind::LiteralValExpression:
            return bind_literal_val_expression(dynamic_cast<Syntax::LiteralValExpressionNode*>(expression));
        case Syntax::SyntaxKind::MemberExpression:
            return bind_member_expression(dynamic_cast<Syntax::MemberExpressionNode*>(expression));
        case Syntax::SyntaxKind::NameExpression:
            return bind_name_expression(dynamic_cast<Syntax::NameExpressionNode*>(expression));
        case Syntax::SyntaxKind::TernaryExpression:
            return bind_ternary_expression(dynamic_cast<Syntax::TernaryExpressionNode*>(expression));
        case Syntax::SyntaxKind::UnaryExpression:
            return bind_unary_expression(dynamic_cast<Syntax::UnaryExpressionNode*>(expression));
        default:
            throw std::runtime_error("Unexpected syntax while binding expression.");
    }

    return bind_error_expression();
}

BoundExpressionNode* Binder::bind_assignment_expression(Syntax::AssignmentExpressionNode* assignment_expression) {
    auto bound_expression = bind_expression(assignment_expression->expression());
    auto bound_identifier_expression = bind_expression(assignment_expression->identifier_expression());

    if (Symbols::TypeSymbol::is_error_type(bound_identifier_expression->type()))
        return bind_error_expression();

    auto lvar_ref = dynamic_cast<BoundVariableReferenceExpressionNode*>(bound_identifier_expression);

    // if the assignee is not an lvalue type, return an error type
    if (lvar_ref == nullptr) 
        return bind_error_expression(); 

    Symbols::VariableSymbol* variable = lvar_ref->variable_reference();

    if (variable->is_const()) {
        m_diagnostics->report_cannot_assign_to_constant(assignment_expression->token(), variable->name());
        return bind_error_expression();
    }

    if (!Symbols::TypeSymbol::are_types_equivalent(bound_expression->type(), lvar_ref->type())) {
        m_diagnostics->report_cannot_assign_type_mismatch(assignment_expression->token(), bound_expression->type()->str(), variable->var_type()->str());
        return bind_error_expression();
    }

    if (bound_expression->type()->attributes().is_array && lvar_ref->type()->attributes().is_array) {
        m_diagnostics->report_invalid_assignment_operator(assignment_expression->token(), lvar_ref->type()->str(), bound_expression->type()->str());
        return bind_error_expression();
    }

    BoundAssignmentOpKind assignment_op_kind;
    bool rewrite = true;
    Syntax::token_type_t rewrite_op_kind;
    switch (assignment_expression->assignment_type()) {
        case Syntax::token_type_t::ASSIGN:
            assignment_op_kind = BoundAssignmentOpKind::ASSIGN;
            rewrite = false;
            break;
        case Syntax::token_type_t::PLUSASSIGN:
            assignment_op_kind = BoundAssignmentOpKind::PLUS_ASSIGN;
            rewrite_op_kind = Syntax::token_type_t::PLUS;
            break;
        case Syntax::token_type_t::MINUSASSIGN:
            assignment_op_kind = BoundAssignmentOpKind::MINUS_ASSIGN;
            rewrite_op_kind = Syntax::token_type_t::MINUS;
            break;
        case Syntax::token_type_t::STARASSIGN:
            assignment_op_kind = BoundAssignmentOpKind::STAR_ASSIGN;
            rewrite_op_kind = Syntax::token_type_t::STAR;
            break;
        case Syntax::token_type_t::SLASHASSIGN:
            assignment_op_kind = BoundAssignmentOpKind::SLASH_ASSIGN;
            rewrite_op_kind = Syntax::token_type_t::SLASH;
            break;
        default:
            return bind_error_expression();
    }

    if (rewrite) {
        auto bound_bin_op = BoundBinaryOperatorNode::Bind(rewrite_op_kind, lvar_ref->type(), bound_expression->type());
        if (bound_bin_op == nullptr) {
            m_diagnostics->report_invalid_assignment_operator(assignment_expression->token(), variable->var_type()->str(), bound_expression->type()->str());
            return bind_error_expression();
        }
    }

    return new BoundAssignmentExpressionNode(assignment_op_kind, lvar_ref, bound_expression);
}

BoundExpressionNode* Binder::bind_binary_expression(Syntax::BinaryExpressionNode* binary_expression) {
    auto bound_left = bind_expression(binary_expression->left_expression());
    auto bound_right = bind_expression(binary_expression->right_expression());

    if (Symbols::TypeSymbol::is_error_type(bound_left->type()) || Symbols::TypeSymbol::is_error_type(bound_right->type()))
        return bind_error_expression();

    auto bound_operator = BoundBinaryOperatorNode::Bind(binary_expression->syntax_token_type(), bound_left->type(), bound_right->type());

    if (bound_operator == nullptr) {
        m_diagnostics->report_invalid_binary_operator(binary_expression->token(), bound_left->type()->str(), bound_right->type()->str());
        return bind_error_expression();
    }

    return new BoundBinaryExpressionNode(bound_operator, bound_left, bound_right);
}

BoundExpressionNode* Binder::bind_call_expression(Syntax::CallExpressionNode* call_expression) {
    Symbols::FunctionSymbol* symbol;
    std::string identifier =  call_expression->name();
    auto exists = m_scope->try_look_up_function(identifier, symbol);
    if (!exists) {
        m_diagnostics->report_function_not_declared(call_expression->token(), identifier);
        return bind_error_expression();
    }
    // if (symbol->kind() != Symbols::SymbolKind::FUNCTION) {
    //     m_diagnostics->report_identifier_is_not_a_function(call_expression->token(), identifier);
    //     return bind_error_expression();
    // }
    
    auto function = dynamic_cast<Symbols::FunctionSymbol*>(symbol);

    if (function->params().size() != call_expression->expressions().size()) {
        bool too_few = function->params().size() > call_expression->expressions().size();
        m_diagnostics->report_wrong_argument_count(call_expression->token(), identifier, too_few, function->params().size(), call_expression->expressions().size());
        return bind_error_expression();
    }
    
    std::vector<BoundExpressionNode*> bound_arguments;
    for (std::vector<Symbols::ParameterSymbol*>::size_type arg_idx = 0; arg_idx < function->params().size(); arg_idx++) {
        auto expected_type = function->params()[arg_idx]->type();
        auto bound_expression = bind_expression(call_expression->expressions()[arg_idx]);
        auto actual_type = bound_expression->type();

        if (!Symbols::TypeSymbol::are_types_equivalent(actual_type, expected_type)) {
            auto token = call_expression->expressions()[arg_idx]->token();
            m_diagnostics->report_incompatible_argument(token, identifier, actual_type->str(), expected_type->str(), int(arg_idx+1));
            return bind_error_expression();
        }

        bound_arguments.push_back(bound_expression);
    }

    return new BoundCallExpressionNode(function, bound_arguments);    
}

BoundExpressionNode* Binder::bind_cast_expression(Syntax::CastExpressionNode* cast_expression) {
    const Symbols::TypeSymbol* type_symbol = nullptr;
    if (!m_scope->try_look_up_type(cast_expression->type(), type_symbol)) {
        m_diagnostics->report_no_definition_for_type_symbol(cast_expression->token(), cast_expression->type());
        return bind_error_expression();
    }

    auto bound_expression = bind_expression(cast_expression->expression());
    if (m_err_flag) return new BoundErrorExpressionNode();

    // can_be_explicitly_casted implicitly requires that both types are numeric types.
    if (!Symbols::TypeSymbol::can_be_explicitly_casted(bound_expression->type(), type_symbol)) {
        m_diagnostics->report_cannot_convert_explicitly(cast_expression->token(), bound_expression->type()->str(), type_symbol->str());
        return bind_error_expression();
    }

    if (bound_expression->type()->attributes().is_const) {
        type_symbol = type_symbol->as_const_type();
    }

    return new BoundCastExpressionNode(type_symbol, bound_expression);
}

BoundExpressionNode* Binder::bind_decrement_expression(Syntax::DecrementExpressionNode* decrement_expression) {
    auto notation = decrement_expression->is_post() ? IDNotation::POSTFIX : IDNotation::PREFIX;
    auto bound_expr = bind_expression(decrement_expression->identifier_expression());
    if (bound_expr->type()->attributes().is_const || bound_expr->type()->attributes().is_array) {
        m_diagnostics->report_cannot_decrement_lval(decrement_expression->token(), bound_expr->type()->str());
        return bind_error_expression();
    }
    return new BoundDecrementExpressionNode(notation, bound_expr);
}

BoundExpressionNode* Binder::bind_increment_expression(Syntax::IncrementExpressionNode* increment_expression) {
    auto notation = increment_expression->is_post() ? IDNotation::POSTFIX : IDNotation::PREFIX;
    auto bound_expr = bind_expression(increment_expression->identifier_expression());
    if (bound_expr->type()->attributes().is_const || bound_expr->type()->attributes().is_array) {
        m_diagnostics->report_cannot_increment_lval(increment_expression->token(), bound_expr->type()->str());
        return bind_error_expression();
    }
    return new BoundIncrementExpressionNode(notation, bound_expr);
}

BoundExpressionNode* Binder::bind_index_expression(Syntax::IndexExpressionNode* index_expression) {
    Symbols::VariableSymbol* variable = nullptr;
    if (m_struct_scope.empty() || m_struct_scope.top() == nullptr) {
        if (!m_scope->try_look_up_variable(index_expression->name(), variable)) {
            m_diagnostics->report_undefined_variable(index_expression->token(), index_expression->name());
            return bind_error_expression();
        }
    } else {
        for (auto member : m_struct_scope.top()->members()) {
            if (member->name() == index_expression->name()) {
                variable = member;
                break;
            }
        }

        if (variable == nullptr) {
            m_diagnostics->report_no_member_in_type(index_expression->token(), index_expression->name(), m_struct_scope.top()->name());
            return bind_error_expression();
        }
    }

    if (!variable->is_array()) {
        m_diagnostics->report_variable_not_array_type(index_expression->token(), index_expression->name());
        return bind_error_expression();
    }

    m_struct_scope.push(nullptr); // probably a terrible way to do this
    auto bound_index_value_expression = bind_expression(index_expression->expression());
    m_struct_scope.pop();
    if (!bound_index_value_expression->type()->attributes().is_integer_type) {
        m_diagnostics->report_array_index_must_be_integer(index_expression->token(), variable->name());
        return bind_error_expression();
    }

    return new BoundIndexExpressionNode(variable, bound_index_value_expression);
}

BoundExpressionNode* Binder::bind_literal_val_expression(Syntax::LiteralValExpressionNode* literal_expression) {
    auto literal_kind = literal_expression->value_type();
    switch (literal_kind) {
        case Syntax::TOKEN_DATA_TYPE::CHAR:
            return new BoundLiteralValExpressionNode(literal_expression->char_value());
        case Syntax::TOKEN_DATA_TYPE::INT:
            return new BoundLiteralValExpressionNode(literal_expression->int_value());
        case Syntax::TOKEN_DATA_TYPE::FLOAT:
            return new BoundLiteralValExpressionNode(literal_expression->float_value());
        case Syntax::TOKEN_DATA_TYPE::STRING:
            return new BoundLiteralValExpressionNode(literal_expression->string_value());
    }
    return new BoundErrorExpressionNode();
}

BoundExpressionNode* Binder::bind_member_expression(Syntax::MemberExpressionNode* member_expression) {
    auto encapsulating_variable_expression = bind_expression(member_expression->encapsulator());
    if (m_err_flag) return new BoundErrorExpressionNode();
    auto encapsulating_var_ref = dynamic_cast<BoundVariableReferenceExpressionNode*>(encapsulating_variable_expression);
    auto encap_type = encapsulating_var_ref->type();
    //auto encapsulating_var_ref_type = encapsulating_var_ref->type();

    // check if encapsulating type is a struct
    //const Symbols::TypeSymbol* encap_type;
    //m_scope->try_look_up_type(encapsulating_var_ref_type->name(), encap_type);
    
    // don't have to check if type exists since that would be caught when binding
    // the name expression of of 'encapsulating_variable_expression'
    if (!encap_type->attributes().is_struct || encap_type->attributes().is_array) {
        m_diagnostics->report_member_base_type_not_struct(member_expression->token(), encap_type->str());
        return bind_error_expression();
    }

    
    Symbols::StructSymbol* struct_symbol;
    m_scope->try_look_up_struct(encap_type->name(), struct_symbol);
    
    m_struct_scope.push(struct_symbol);
    auto bound_expression = bind_expression(member_expression->member());
    m_struct_scope.pop();
    if (m_err_flag) return new BoundErrorExpressionNode();

    auto bound_var_ref_expr = dynamic_cast<BoundVariableReferenceExpressionNode*>(bound_expression);

    return new BoundMemberAccessExpressionNode(bound_var_ref_expr, encapsulating_var_ref);
}

BoundExpressionNode* Binder::bind_name_expression(Syntax::NameExpressionNode* name_expression) {
    Symbols::VariableSymbol* variable = nullptr;
    if (m_struct_scope.empty() || m_struct_scope.top() == nullptr) {
        if (!m_scope->try_look_up_variable(name_expression->name(), variable)) {
            m_diagnostics->report_undefined_variable(name_expression->token(), name_expression->name());
            return bind_error_expression();
        }
    } else {
        for (auto member : m_struct_scope.top()->members()) {
            if (member->name() == name_expression->name()) {
                variable = member;
                break;
            }
        }

        if (variable == nullptr) {
            m_diagnostics->report_no_member_in_type(name_expression->token(), name_expression->name(), m_struct_scope.top()->name());
            return bind_error_expression();
        }
    }

    return new BoundVariableReferenceExpressionNode(variable);
}

BoundExpressionNode* Binder::bind_ternary_expression(Syntax::TernaryExpressionNode* ternary_expression) {
    auto bound_conditional = bind_expression(ternary_expression->conditional_expression());
    if (!Symbols::TypeSymbol::is_conditional_type(bound_conditional->type())) {
        m_diagnostics->report_integer_type_required(ternary_expression->token(), bound_conditional->type()->str());
        return bind_error_expression();
    }

    auto bound_left = bind_expression(ternary_expression->true_expression(),true);
    auto bound_right = bind_expression(ternary_expression->false_expression(),true);
    auto left_type = bound_left->type();
    auto right_type = bound_right->type();

    if (Symbols::TypeSymbol::is_error_type(left_type) || Symbols::TypeSymbol::is_error_type(right_type))
        return bind_error_expression();

    if (!Symbols::TypeSymbol::are_types_equivalent(left_type, right_type) && 
        !Symbols::TypeSymbol::are_types_equivalent(right_type, left_type)) {
        
        m_diagnostics->report_incompatible_operand_types(ternary_expression->token(), left_type->str(), right_type->str());
        return bind_error_expression();
    }

    auto ternary_ret_type = Symbols::TypeSymbol::get_wider_type(left_type, right_type);

    return new BoundTernaryExpressionNode(ternary_ret_type, bound_conditional, bound_left, bound_right);
}

BoundExpressionNode* Binder::bind_unary_expression(Syntax::UnaryExpressionNode* unary_expression) {
    auto bound_expression = bind_expression(unary_expression->expression());

    if (Symbols::TypeSymbol::is_error_type(bound_expression->type()))
        return bind_error_expression();

    auto bound_operator = BoundUnaryOperatorNode::Bind(unary_expression->syntax_token_type(), bound_expression->type());

    if (bound_operator == nullptr) {
        m_diagnostics->report_invalid_unary_operator(unary_expression->token(), bound_expression->type()->str());
        return bind_error_expression();
    }

    return new BoundUnaryExpressionNode(bound_operator, bound_expression);
}



const Symbols::TypeSymbol* Binder::bind_type_clause(std::string type_name) {
    const Symbols::TypeSymbol* type;
    if (!m_scope->try_look_up_type(type_name, type)) {
        return &Symbols::TypeSymbol::Error;
    }
    return type;
}

}