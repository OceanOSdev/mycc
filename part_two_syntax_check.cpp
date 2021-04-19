#include "part_two_syntax_check.h"
#include "logging/logger.h"
#include "syntax/syntax_kind.h"
#include "syntax/formal_parameter_node.h"
#include "syntax/expression_node.h"
#include "syntax/cast_expression_node.h"
#include "syntax/ternary_expression_node.h"
#include "syntax/binary_expression_node.h"
#include "syntax/unary_expression_node.h"
#include "syntax/call_expression_node.h"
#include "syntax/name_expression_node.h"
#include "syntax/index_expression_node.h"
#include "syntax/member_expression_node.h"
#include "syntax/assignment_expression_node.h"
#include "syntax/decrement_expression_node.h"
#include "syntax/increment_expression_node.h"
#include "syntax/partial_variable_declaration_node.h"
#include "syntax/variable_group_declaration_node.h"
#include "syntax/literal_val_expression_node.h"
#include "syntax/statement_node.h"
#include "syntax/expression_statement_node.h"
#include "syntax/continue_statement_node.h"
#include "syntax/break_statement_node.h"
#include "syntax/return_statement_node.h"
#include "syntax/if_statement_node.h"
#include "syntax/block_statement_node.h"
#include "syntax/for_statement_node.h"
#include "syntax/while_statement_node.h"
#include "syntax/do_while_statement_node.h"
#include "syntax/struct_declaration_node.h"
#include "syntax/function_declaration_node.h"
#include "syntax/function_prototype_node.h"
#include "syntax/function_definition_node.h"
#include "syntax/global_declaration_node.h"
#include "syntax/local_declaration_node.h"
#include "syntax/global_struct_declaration_node.h"
#include "syntax/global_variable_group_declaration_node.h"
#include "syntax/translation_unit_node.h"
#include "syntax/program_node.h"

PartTwoSyntaxPrinter::PartTwoSyntaxPrinter(Syntax::ProgramNode* prgm, Logging::Logger* logger) :
    m_prgm(prgm), m_logger(logger) {}


PartTwoSyntaxPrinter::~PartTwoSyntaxPrinter() {
    //TODO: dispose of the pointer collections
}


void PartTwoSyntaxPrinter::visit_global_declaration_node(Syntax::GlobalDeclarationNode* g) {
    if (auto gvgd = dynamic_cast<Syntax::GlobalVariableGroupDeclarationNode*>(g)) {
        m_symbols.back().glob_vars.push_back(gvgd->variable_group());

    } else if (auto gsd = dynamic_cast<Syntax::GlobalStructDeclarationNode*>(g)) {
        m_symbols.back().glob_structs.push_back(gsd->struct_declaration());
        
    } else if (auto fp = dynamic_cast<Syntax::FunctionPrototypeNode*>(g)) {
        m_symbols.back().func_data.push_back(fp);

    } else if (auto fd = dynamic_cast<Syntax::FunctionDefinitionNode*>(g)) {
        m_symbols.back().func_data.push_back(fd);
    }
}

void PartTwoSyntaxPrinter::visit_tranlation_unit_node(Syntax::TranslationUnitNode* t) {
    std::vector<Syntax::GlobalDeclarationNode*>::iterator gd_iter;
    auto gds = t->global_declarations();
    for (auto gd : gds) {
        visit_node(gd);
    }
}

void PartTwoSyntaxPrinter::visit_node(Syntax::SyntaxNode* n) {
    if (auto gd = dynamic_cast<Syntax::GlobalDeclarationNode*>(n)) {
        visit_global_declaration_node(gd);

    } else if (auto tu = dynamic_cast<Syntax::TranslationUnitNode*>(n)) {
        visit_tranlation_unit_node(tu);
    }
}

std::vector<std::string> PartTwoSyntaxPrinter::var_to_string(std::vector<Syntax::VariableGroupDeclarationNode*> variables) {
    std::vector<std::string> varids;
    for (auto varlist : variables) {
        for (auto var : varlist->partial_variable_group()) {
            std::string name = var->identifier();
            if (var->is_array()) name += "[]";
            varids.push_back(name);
        }
    }
    return varids;
}

std::vector<std::string> PartTwoSyntaxPrinter::param_to_string(std::vector<Syntax::FormalParameterNode*> params) {
    std::vector<std::string> paramids;
    for (auto param : params) {
        std::string name = param->param_name();
        if (param->is_array()) name += "[]";
        paramids.push_back(name);
    }
    return paramids;
}

void PartTwoSyntaxPrinter::log_vars_helper(std::vector<std::string> vars) {
    std::vector<std::string>::iterator iter;
    auto last = std::prev(vars.end(),1);
    for (iter = vars.begin(); iter != vars.end(); iter++) {
        m_logger->log_info(*iter);
        if (iter != last) m_logger->log_info(", ");
    }
    m_logger->log_info_nl("");
}

void PartTwoSyntaxPrinter::log_func_prototype(Syntax::FunctionPrototypeNode* p) {
    auto fdecl = p->function_declaration();
    m_logger->log_info("Prototype ");
    m_logger->log_info(fdecl->function_name());
    if (!fdecl->parameters().empty()) {
        m_logger->log_info("\n\t");
        std::vector<std::string> paramstrs = param_to_string(fdecl->parameters());
        m_logger->log_info("Parameters: ");
        log_vars_helper(paramstrs);
        m_logger->log_info_nl("");
    } else 
        m_logger->log_info_nl("\n");
}

void PartTwoSyntaxPrinter::log_func_definition(Syntax::FunctionDefinitionNode* d) {
    auto fdecl = d->function_declaration();
    m_logger->log_info("Function ");
    m_logger->log_info(fdecl->function_name() + "\n");
    if (!fdecl->parameters().empty()) {
        m_logger->log_info("\t");
        std::vector<std::string> paramstrs = param_to_string(fdecl->parameters());
        m_logger->log_info("Parameters: ");
        log_vars_helper(paramstrs);
    }
    if (!d->local_declarations().empty()) {
        std::vector<std::string> vars, structs;
        for (auto ld : d->local_declarations()) {
            if (auto v = dynamic_cast<Syntax::VariableGroupDeclarationNode*>(ld)) {
                std::vector<Syntax::VariableGroupDeclarationNode*> s = {v};
                auto strs = var_to_string(s);
                vars.insert(vars.end(), strs.begin(), strs.end());
            } else if (auto s = dynamic_cast<Syntax::StructDeclarationNode*>(ld)) {
                structs.push_back(s->struct_name());
            }
        }
        if (!structs.empty()) {
            m_logger->log_info("\tLocal structs: ");
            log_vars_helper(structs);
        }
        if (!vars.empty()) {
            m_logger->log_info("\tLocal variables: ");
            log_vars_helper(vars);
        }
    }

    m_logger->log_info_nl("");
}

void PartTwoSyntaxPrinter::log_func_data(PartTwoSymbolTable tbl) {
    for (auto data : tbl.func_data) {
        if (std::holds_alternative<Syntax::FunctionPrototypeNode*>(data)) {
            log_func_prototype(std::get<Syntax::FunctionPrototypeNode*>(data));
        } else if (std::holds_alternative<Syntax::FunctionDefinitionNode*>(data)) {
            log_func_definition(std::get<Syntax::FunctionDefinitionNode*>(data));
        }       
    }
}

void PartTwoSyntaxPrinter::log_glob_structs(PartTwoSymbolTable tbl) {
    for (auto glob_struct : tbl.glob_structs) {
        m_logger->log_info("Global stuct ");
        m_logger->log_info(glob_struct->struct_name());
        m_logger->log_info("\n\t");
        std::vector<std::string> member_names = var_to_string(glob_struct->struct_members());
        log_vars_helper(member_names);
        m_logger->log_info_nl(" ");
    } 
}

void PartTwoSyntaxPrinter::log_glob_vars(PartTwoSymbolTable tbl) {
    m_logger->log_info("Global variables\n\t");
    std::vector<std::string> glob_var_names = var_to_string(tbl.glob_vars);
    log_vars_helper(glob_var_names);
    m_logger->log_info_nl("");
}


void PartTwoSyntaxPrinter::print_info() {
    for (auto unit : m_prgm->units()) {
        PartTwoSymbolTable tbl;
        m_symbols.push_back(tbl);
        visit_node(unit);
    }

    for (auto symtab : m_symbols) {
        log_glob_structs(symtab);
        log_glob_vars(symtab);
        log_func_data(symtab);
    }


}