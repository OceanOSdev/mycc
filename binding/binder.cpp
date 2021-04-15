#include <stdexcept>
#include <set>
#include "binder.h"
#include "bound_statement_node.h"
#include "bound_expression_statement_node.h"
#include "bound_error_expression_node.h"
#include "bound_scope.h"
#include "bound_global_declaration_node.h"
#include "bound_global_statement_node.h"
#include "bound_function_definition_node.h"
#include "../syntax/statement_node.h"
#include "../logger.h"
#include "../diagnostics.h"
#include "../symbols/type_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/function_symbol.h"
#include "../symbols/variable_symbol.h"
#include "../symbols/struct_symbol.h"
#include "../symbols/parameter_symbol.h"
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
        }
        case Syntax::SyntaxKind::FunctionDefinition:
            break;
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

        params.push_back(new Symbols::ParameterSymbol(parameter_syntax->param_name(), param_type, parameter_syntax->is_array(), parameter_syntax->is_const()));
    }

    std::string func_type_id = declaration->type_identifier();
    auto func_type = bind_type_clause(func_type_id);
    if (Symbols::TypeSymbol::are_types_equivalent(func_type, &Symbols::TypeSymbol::Error)) {
        if (declaration->is_struct()) func_type_id = "struct " + func_type_id;
        m_diagnostics->report_no_definition_for_type_symbol(declaration->token(), func_type_id);
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
    bind_function_declaration(function_definition->function_declaration());
    return nullptr;
}

BoundStatementNode* Binder::bind_statement(Syntax::StatementNode* statement) {
    auto syntaxKind = statement->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::BlockStatement: 
            break;
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
            break;
        case Syntax::SyntaxKind::IfStatement: 
            break;
        case Syntax::SyntaxKind::ReturnStatement: 
            break;
        case Syntax::SyntaxKind::WhileStatement: 
            break;
        default:
            throw std::runtime_error("Unexpected syntax while binding statement.");
    }

    return nullptr;
}

BoundStatementNode* Binder::bind_expression_statement(Syntax::ExpressionStatementNode* expressionStatement) {
    auto expression = bind_expression(expressionStatement->expression(), true);
    return new BoundExpressionStatementNode(expression);
}

BoundExpressionNode* Binder::bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid) {
    auto result = bind_expression_internal(expression);
    if (!canBeVoid && Symbols::TypeSymbol::are_types_equivalent(result->type(), &Symbols::TypeSymbol::Void)) {
        m_diagnostics->report_expression_cannot_be_void(expression->token());
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
            break;
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


const Symbols::TypeSymbol* Binder::bind_type_clause(std::string type_name) {
    const Symbols::TypeSymbol* type;
    if (!m_scope->try_look_up_type(type_name, type)) {
        return &Symbols::TypeSymbol::Error;
    }
    return type;
}

}