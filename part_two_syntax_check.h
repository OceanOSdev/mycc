#ifndef PART_TWO_SYNTAX_CHECK_H
#define PART_TWO_SYNTAX_CHECK_H
#include "pch.h"
#include "logger.h"
#include <vector>
#include <string>
#include <variant>

using func_wrapper = std::variant<Syntax::FunctionPrototypeNode*, Syntax::FunctionDefinitionNode*>;

using PartTwoSymbolTable = struct part_two_symbol_table {
    std::vector<Syntax::VariableGroupDeclarationNode*> glob_vars;
    std::vector<Syntax::StructDeclarationNode*> glob_structs;
    std::vector<func_wrapper> func_data;
};


class PartTwoSyntaxPrinter {
private: 
    Syntax::ProgramNode* m_prgm;
    Logger* m_logger;
    std::vector<PartTwoSymbolTable> m_symbols;

    void visit_node(Syntax::SyntaxNode* n);
    void visit_global_declaration_node(Syntax::GlobalDeclarationNode* n);
    void visit_tranlation_unit_node(Syntax::TranslationUnitNode* n);
    void log_glob_vars(PartTwoSymbolTable tbl);
    void log_glob_structs(PartTwoSymbolTable tbl);
    void log_func_data(PartTwoSymbolTable tbl);
    void log_func_prototype(Syntax::FunctionPrototypeNode* p);
    void log_func_definition(Syntax::FunctionDefinitionNode* d);
    void log_vars_helper(std::vector<std::string> vars);
    std::vector<std::string> var_to_string(std::vector<Syntax::VariableGroupDeclarationNode*> variables);
    std::vector<std::string> param_to_string(std::vector<Syntax::FormalParameterNode*> params);
    
public:
    PartTwoSyntaxPrinter(Syntax::ProgramNode* prgm, Logger* logger);
    ~PartTwoSyntaxPrinter();

    void print_info();
};

#endif