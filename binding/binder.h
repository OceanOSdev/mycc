#pragma once
#ifndef BINDER_H
#define BINDER_H

#include <vector>
#include <string>
#include <stack>
#include <tuple>

namespace Logging {
    class DiagnosticsList;
    struct PartThreeInfoList;
}

namespace Syntax {
    // Statement Syntax Forward Declarations
    class StatementNode;
    class ExpressionStatementNode;
    class BlockStatementNode;
    class ReturnStatementNode;
    class VariableGroupDeclarationNode;
    class StructDeclarationNode;
    class BreakStatementNode;
    class ContinueStatementNode;
    class DoWhileStatementNode;
    class ForStatementNode;
    class IfStatementNode;
    class WhileStatementNode;
    
    // Expression Syntax Forward Declarations
    class ExpressionNode;
    class LiteralValExpressionNode;
    class IndexExpressionNode;
    class MemberExpressionNode;
    class NameExpressionNode;
    class CastExpressionNode;
    class BinaryExpressionNode;
    class UnaryExpressionNode;
    class AssignmentExpressionNode;
    class CallExpressionNode;
    class TernaryExpressionNode;
    class IncrementExpressionNode;
    class DecrementExpressionNode;

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
class BoundLabel;

/*
 * structured way to record type info for part three.
 */
struct PartThreeInfoList;

class Binder {
private:
    Logging::DiagnosticsList* m_diagnostics;
    Logging::PartThreeInfoList* m_part_three_info_list;
    //std::vector<std::string> m_part_three_info_list;
    std::vector<BoundGlobalDeclarationNode*> m_global_decls;
    bool m_err_flag;
    int m_label_idx; // label counter for loops and stuff
    BoundScope* m_scope;
    Symbols::FunctionSymbol* m_current_function; // set when binding methods in function body
    std::stack<Symbols::StructSymbol*> m_struct_scope; // for when binding name and index expressions of submembers, will be empty otherwise.
    std::stack<std::tuple<BoundLabel*, BoundLabel*>> m_loop_stack; // for binding break and continue statements, we check if they are in a loop first

    static BoundScope* init_global_scope();
    void set_current_function_scope(Symbols::FunctionSymbol* function_symbol);

    /* global bindings */
    Symbols::FunctionSymbol* bind_function_declaration(Syntax::FunctionDeclarationNode* declaration);
    void bind_function_prototype(Syntax::FunctionPrototypeNode* prototype);
    BoundFunctionDefinitionNode* bind_function_definition(Syntax::FunctionDefinitionNode* function_definition);
    
    /* statement bindings */
    BoundStatementNode* bind_error_statement();
    BoundStatementNode* bind_statement(Syntax::StatementNode* statement, bool is_global_scope = false);
    BoundStatementNode* bind_expression_statement(Syntax::ExpressionStatementNode* expression_statement);
    BoundStatementNode* bind_block_statement(Syntax::BlockStatementNode* block_statement, bool create_new_scope = true);
    BoundStatementNode* bind_variable_group_declaration(Syntax::VariableGroupDeclarationNode* variable_group, bool is_global_scope = false);
    BoundStatementNode* bind_return_statement(Syntax::ReturnStatementNode* return_statement);
    BoundStatementNode* bind_struct_declaration(Syntax::StructDeclarationNode* struct_declaration);
    BoundStatementNode* bind_break_statement(Syntax::BreakStatementNode* break_statement);
    BoundStatementNode* bind_continue_statement(Syntax::ContinueStatementNode* continue_statement);
    BoundStatementNode* bind_do_while_statement(Syntax::DoWhileStatementNode* do_while_statement);
    BoundStatementNode* bind_for_statement(Syntax::ForStatementNode* for_statement);
    BoundStatementNode* bind_if_statement(Syntax::IfStatementNode* if_statement);
    BoundStatementNode* bind_while_statement(Syntax::WhileStatementNode* while_statement);
    BoundStatementNode* bind_loop_body(Syntax::StatementNode* body, BoundLabel*& break_label, BoundLabel*& continue_label);

    /* expression bindings */
    BoundExpressionNode* bind_error_expression();
    BoundExpressionNode* bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid = false);
    BoundExpressionNode* bind_expression_internal(Syntax::ExpressionNode* expression);
    BoundExpressionNode* bind_literal_val_expression(Syntax::LiteralValExpressionNode* literal_expression);
    BoundExpressionNode* bind_name_expression(Syntax::NameExpressionNode* name_expression);
    BoundExpressionNode* bind_member_expression(Syntax::MemberExpressionNode* member_expression);
    BoundExpressionNode* bind_index_expression(Syntax::IndexExpressionNode* index_expression);
    BoundExpressionNode* bind_cast_expression(Syntax::CastExpressionNode* cast_expression);
    BoundExpressionNode* bind_binary_expression(Syntax::BinaryExpressionNode* binary_expression);
    BoundExpressionNode* bind_unary_expression(Syntax::UnaryExpressionNode* unary_expression);
    BoundExpressionNode* bind_assignment_expression(Syntax::AssignmentExpressionNode* assignment_expression);
    BoundExpressionNode* bind_call_expression(Syntax::CallExpressionNode* call_expression);
    BoundExpressionNode* bind_ternary_expression(Syntax::TernaryExpressionNode* ternary_expression);
    BoundExpressionNode* bind_increment_expression(Syntax::IncrementExpressionNode* increment_expression);
    BoundExpressionNode* bind_decrement_expression(Syntax::DecrementExpressionNode* decrement_expression);

    /* symbol bindings */
    const Symbols::TypeSymbol* bind_type_clause(std::string type_name);

public:
    Binder(Logging::DiagnosticsList* diagnostics, BoundScope* parent);

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
    Logging::DiagnosticsList* diagnostics() const;

    /*
     * Custom containing the info to output
     * for part 3.
     */
    Logging::PartThreeInfoList* part_three_info_list() const;

    /*
     * Bool flag which returns true if there was an error
     * during binding.
     */
    bool err_flag() const;
};

}

#endif