/* LIB INCLUDES */
#include <stdexcept>
#include <set>
#include <algorithm>

/* HEADER INCLUDE */
#include "binder.h"

/* BOUND TREE NODE INCLUDES */
#include "bound_scope.h"
#include "bound_global_declaration_node.h"
#include "bound_global_statement_node.h"
#include "bound_function_definition_node.h"
#include "bound_block_statement_node.h"

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
#include "../syntax/syntax_kind.h"
#include "../syntax/syntax_token.h"
#include "../syntax/global_declaration_node.h"
#include "../syntax/global_variable_group_declaration_node.h"
#include "../syntax/global_struct_declaration_node.h"
#include "../syntax/function_prototype_node.h"
#include "../syntax/function_definition_node.h"
#include "../syntax/formal_parameter_node.h"
#include "../syntax/program_node.h"
#include "../syntax/block_statement_node.h"

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


const Symbols::TypeSymbol* Binder::bind_type_clause(std::string type_name) {
    const Symbols::TypeSymbol* type;
    if (!m_scope->try_look_up_type(type_name, type)) {
        return &Symbols::TypeSymbol::Error;
    }
    return type;
}

}