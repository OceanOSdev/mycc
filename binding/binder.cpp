/* LIB INCLUDES */
#include <stdexcept>
#include <set>

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
#include "bound_variable_group_declaration_node.h"
#include "bound_literal_val_expression_node.h"
#include "bound_index_expression_node.h"

/* UTILITY INCLUDES */
#include "../logger.h"
#include "../diagnostics.h"

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
#include "../syntax/variable_group_declaration_node.h"
#include "../syntax/partial_variable_declaration_node.h"
#include "../syntax/literal_val_expression_node.h"
#include "../syntax/index_expression_node.h"
#include "../syntax/program_node.h"


namespace Binding {

Binder::Binder(DiagnosticsList* diagnostics, BoundScope* parent) : 
    m_diagnostics(diagnostics), m_scope(new BoundScope(parent)) {

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
    DiagnosticsList* diag = new DiagnosticsList();
    auto binder = new Binder(diag, scope);

    // Again, since we only really support one input
    // file, we only need to grab the first translation
    // unit.
    auto translationUnit = program->units()[0];
    for (auto dec : translationUnit->global_declarations()) {
        binder->bind_global_declaration(dec);
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
DiagnosticsList* Binder::diagnostics() const {
    return m_diagnostics;
}

/*
 * List of strings containing the info to output
 * for part 3.
 */
std::vector<std::string> Binder::part_three_info_list() const {
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
        if (Symbols::TypeSymbol::are_types_equivalent(param_type, &Symbols::TypeSymbol::Error)) {
            if (parameter_syntax->is_struct()) type_symbol_id = "struct " + type_symbol_id; // probably not the best way to accomplish this.
            m_diagnostics->report_no_definition_for_type_symbol(parameter_syntax->token(), type_symbol_id);
            m_err_flag = true;
            return nullptr;
        }

        bool is_array = parameter_syntax->is_array();
        params.push_back(new Symbols::ParameterSymbol(parameter_syntax->param_name(), 
                                                      is_array ? param_type->as_array_type() : param_type, 
                                                      is_array, 
                                                      parameter_syntax->is_const()));
    }

    std::string func_type_id = declaration->type_identifier();
    auto func_type = bind_type_clause(func_type_id);
    if (Symbols::TypeSymbol::are_types_equivalent(func_type, &Symbols::TypeSymbol::Error)) {
        m_diagnostics->report_no_definition_for_type_symbol(declaration->token(), func_type->str());
        m_err_flag = true;
        return nullptr;
    }

    return new Symbols::FunctionSymbol(declaration->function_name(), func_type, params);
}

void Binder::bind_function_prototype(Syntax::FunctionPrototypeNode* prototype) {
    auto symbol = bind_function_declaration(prototype->function_declaration());
    if (!m_scope->try_declare_function(symbol)) {
        m_diagnostics->report_conflicting_function_declarations(prototype->token(), symbol->name());
        m_err_flag = true;
    }
}

BoundFunctionDefinitionNode* Binder::bind_function_definition(Syntax::FunctionDefinitionNode* function_definition) {
    auto symbol = bind_function_declaration(function_definition->function_declaration());
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
    auto bound_body = bind_statement(body_block_syntax);
    set_current_function_scope(nullptr);
    return new BoundFunctionDefinitionNode(symbol, dynamic_cast<BoundBlockStatementNode*>(bound_body));
}

BoundStatementNode* Binder::bind_error_statement() const {
    return new BoundExpressionStatementNode(new BoundErrorExpressionNode());
}

BoundStatementNode* Binder::bind_statement(Syntax::StatementNode* statement) {
    auto syntaxKind = statement->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::BlockStatement: 
            return bind_block_statement(dynamic_cast<Syntax::BlockStatementNode*>(statement));
        case Syntax::SyntaxKind::BreakStatement: 
            break;
        case Syntax::SyntaxKind::ContinueStatement: 
            break;
        case Syntax::SyntaxKind::DoWhileStatement: 
            break;
        case Syntax::SyntaxKind::ExpressionStatement:
            return bind_expression_statement(dynamic_cast<Syntax::ExpressionStatementNode*>(statement)); 
        case Syntax::SyntaxKind::ForStatement: 
            break;
        case Syntax::SyntaxKind::StructDeclaration: 
            break;
        case Syntax::SyntaxKind::VariableDeclaration: 
            return bind_variable_group_declaration(
                dynamic_cast<Syntax::VariableGroupDeclarationNode*>(statement)
            );
        case Syntax::SyntaxKind::IfStatement: 
            break;
        case Syntax::SyntaxKind::ReturnStatement: 
            return bind_return_statement(dynamic_cast<Syntax::ReturnStatementNode*>(statement));
        case Syntax::SyntaxKind::WhileStatement: 
            break;
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

BoundStatementNode* Binder::bind_expression_statement(Syntax::ExpressionStatementNode* expression_statement) {
    auto expression = bind_expression(expression_statement->expression(), true);
    return new BoundExpressionStatementNode(expression);
}

BoundStatementNode* Binder::bind_variable_group_declaration(Syntax::VariableGroupDeclarationNode* variable_group) {
    std::vector<BoundVariableDeclarationNode*> bound_variables;
    auto type_symbol_id = variable_group->type();
    auto var_type = bind_type_clause(type_symbol_id);
    if (Symbols::TypeSymbol::are_types_equivalent(var_type, &Symbols::TypeSymbol::Error)) {
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
        auto variable_symbol = new Symbols::VariableSymbol(ident, 
                                                            is_array ? var_type->as_array_type() : var_type,
                                                            is_array,
                                                            partial_dec->array_length(),
                                                            variable_group->is_const());
        
        auto added_to_scope = m_scope->try_declare_variable(variable_symbol);
        if (!added_to_scope) {
            m_diagnostics->report_variable_already_declared(partial_dec->token(), variable_symbol->name(), m_current_function == nullptr);
            m_err_flag = true;
        }

        bound_variables.push_back(new BoundVariableDeclarationNode(variable_symbol, initial_val));
    }

    return new BoundVariableGroupDeclarationNode(bound_variables);
}

BoundStatementNode* Binder::bind_return_statement(Syntax::ReturnStatementNode* return_statement) {
    auto bound_expression = return_statement->is_empty_return() ? nullptr : bind_expression(return_statement->expression());

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

BoundExpressionNode* Binder::bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid) {
    auto result = bind_expression_internal(expression);
    if (!canBeVoid && Symbols::TypeSymbol::are_types_equivalent(result->type(), &Symbols::TypeSymbol::Void)) {
        m_diagnostics->report_expression_cannot_be_void(expression->token());
        m_err_flag = true;
        return new BoundErrorExpressionNode();
    }
    return result;
}


BoundExpressionNode* Binder::bind_expression_internal(Syntax::ExpressionNode* expression) {
    auto syntaxKind = expression->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::AssignmentExpression:
            break;
        case Syntax::SyntaxKind::BinaryExpression:
            break;
        case Syntax::SyntaxKind::CallExpression:
            break;
        case Syntax::SyntaxKind::CastExpression:
            break;
        case Syntax::SyntaxKind::DecrementExpression:
            break;
        case Syntax::SyntaxKind::IncrementExpression:
            break;
        case Syntax::SyntaxKind::IndexExpression:
            break;
        case Syntax::SyntaxKind::LiteralValExpression:
            return bind_literal_val_expression(dynamic_cast<Syntax::LiteralValExpressionNode*>(expression));
        case Syntax::SyntaxKind::MemberExpression:
            break;
        case Syntax::SyntaxKind::NameExpression:
            break;
        case Syntax::SyntaxKind::TernaryExpression:
            break;
        case Syntax::SyntaxKind::UnaryExpression:
            break;
        default:
            throw std::runtime_error("Unexpected syntax while binding expression.");
    }

    return nullptr;
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


const Symbols::TypeSymbol* Binder::bind_type_clause(std::string type_name) {
    const Symbols::TypeSymbol* type;
    if (!m_scope->try_look_up_type(type_name, type)) {
        return &Symbols::TypeSymbol::Error;
    }
    return type;
}

}