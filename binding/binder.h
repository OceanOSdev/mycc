#pragma once
#ifndef BINDER_H
#define BINDER_H

#include <vector>
#include <string>

class DiagnosticsList;

namespace Syntax {
    class ProgramNode;
    class StatementNode;
    class ExpressionStatementNode;
    class ExpressionNode;
    class FunctionPrototypeNode;
    class FunctionDefinitionNode;
    class FunctionDeclarationNode;
    class GlobalDeclarationNode;
}

namespace Symbols {
    class TypeSymbol;
    class FunctionSymbol;
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

    static BoundScope* init_global_scope();

    /* global bindings */
    
    Symbols::FunctionSymbol* bind_function_declaration(Syntax::FunctionDeclarationNode* declaration);
    void bind_function_prototype(Syntax::FunctionPrototypeNode* prototype);
    BoundFunctionDefinitionNode* bind_function_definition(Syntax::FunctionDefinitionNode* function_definition);


    /* statement bindings */

    BoundStatementNode* bind_statement(Syntax::StatementNode* statement);
    BoundStatementNode* bind_expression_statement(Syntax::ExpressionStatementNode* expressionStatement);

    /* expression bindings */

    BoundExpressionNode* bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid = false);
    BoundExpressionNode* bind_expression_internal(Syntax::ExpressionNode* expression);

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