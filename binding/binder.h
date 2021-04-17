#pragma once
#ifndef BINDER_H
#define BINDER_H

#include <vector>
#include <string>
#include <stack>

class DiagnosticsList;

namespace Syntax {
    // Statement Syntax Forward Declarations
    class StatementNode;
    class ExpressionStatementNode;
    class BlockStatementNode;
    class ReturnStatementNode;
    class VariableGroupDeclarationNode;
    class StructDeclarationNode;
    
    // Expression Syntax Forward Declarations
    class ExpressionNode;
    class LiteralValExpressionNode;
    class IndexExpressionNode;
    class MemberExpressionNode;
    class NameExpressionNode;

    // Global Syntax Forward Declarations
    class FunctionPrototypeNode;
    class FunctionDefinitionNode;
    class FunctionDeclarationNode;
    class GlobalDeclarationNode;
    class ProgramNode;
}

namespace Symbols {
    class TypeSymbol;
    class FunctionSymbol;
    class StructSymbol;
}

namespace Binding {
// Forward decs
class BoundStatementNode;
class BoundExpressionNode;
class BoundGlobalDeclarationNode;
class BoundFunctionDefinitionNode;
class BoundScope;

class Binder {
private:
    DiagnosticsList* m_diagnostics;
    std::vector<std::string> m_part_three_info_list;
    std::vector<BoundGlobalDeclarationNode*> m_global_decls;
    bool m_err_flag;
    BoundScope* m_scope;
    Symbols::FunctionSymbol* m_current_function; // set when binding methods in function body
    std::stack<Symbols::StructSymbol*> m_struct_scope; // for when binding name and index expressions of submembers, will be empty otherwise.

    static BoundScope* init_global_scope();
    void set_current_function_scope(Symbols::FunctionSymbol* function_symbol);

    /* global bindings */
    Symbols::FunctionSymbol* bind_function_declaration(Syntax::FunctionDeclarationNode* declaration);
    void bind_function_prototype(Syntax::FunctionPrototypeNode* prototype);
    BoundFunctionDefinitionNode* bind_function_definition(Syntax::FunctionDefinitionNode* function_definition);
    
    /* statement bindings */
    BoundStatementNode* bind_error_statement() const;
    BoundStatementNode* bind_statement(Syntax::StatementNode* statement);
    BoundStatementNode* bind_expression_statement(Syntax::ExpressionStatementNode* expression_statement);
    BoundStatementNode* bind_block_statement(Syntax::BlockStatementNode* block_statement, bool create_new_scope = true);
    BoundStatementNode* bind_variable_group_declaration(Syntax::VariableGroupDeclarationNode* variable_group);
    BoundStatementNode* bind_return_statement(Syntax::ReturnStatementNode* return_statement);
    BoundStatementNode* bind_struct_declaration(Syntax::StructDeclarationNode* struct_declaration);

    /* expression bindings */
    BoundExpressionNode* bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid = false);
    BoundExpressionNode* bind_expression_internal(Syntax::ExpressionNode* expression);
    BoundExpressionNode* bind_literal_val_expression(Syntax::LiteralValExpressionNode* literal_expression);
    BoundExpressionNode* bind_name_expression(Syntax::NameExpressionNode* name_expression);
    BoundExpressionNode* bind_member_expression(Syntax::MemberExpressionNode* member_expression);
    BoundExpressionNode* bind_index_expression(Syntax::IndexExpressionNode* index_expression);

    /* type bindings */
    const Symbols::TypeSymbol* bind_type_clause(std::string type_name);
public:
    Binder(DiagnosticsList* diagnostics, BoundScope* parent);

    /*
     * For right now, don't return anything since I'm not sure
     * what to include in the BoundProgram.
     */
    static Binder* bind_program(Syntax::ProgramNode* program);

    void bind_global_declaration(Syntax::GlobalDeclarationNode* gdn);

    /*
     * List of top level statements and declarations in the program.
     */
    std::vector<BoundGlobalDeclarationNode*> global_decls() const;

    /*
     * List of diagnostics describing any errors that may
     * have occured during binding.
     */
    DiagnosticsList* diagnostics() const;

    /*
     * List of strings containing the info to output
     * for part 3.
     */
    std::vector<std::string> part_three_info_list() const;

    /*
     * Bool flag which returns true if there was an error
     * during binding.
     */
    bool err_flag() const;
};

}

#endif