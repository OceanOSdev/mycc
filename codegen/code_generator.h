#pragma once
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <fstream>
#include <ostream>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace Binding {
    class BoundFunctionDefinitionNode;
    class BoundStatementNode;
    class BoundBlockStatementNode;
    class BoundLabelStatementNode;
    class BoundLabel;
    class BoundGotoStatementNode;
    class BoundConditionalGotoStatementNode;
    class BoundReturnStatementNode;
    class BoundVariableGroupDeclarationNode;
    class BoundVariableDeclarationNode;
    class BoundStructDeclarationNode;

    class BoundAssignmentExpressionNode;
    class BoundCallExpressionNode; 
    class BoundCastExpressionNode; 
    class BoundVariableReferenceExpressionNode;
    class BoundIncrementExpressionNode;
    class BoundDecrementExpressionNode;
    class BoundBinaryExpressionNode;
    class BoundUnaryExpressionNode;
    class BoundIndexExpressionNode;
    class BoundExpressionNode;

    class BoundBinaryOperatorNode;
}

namespace Symbols {
    class FunctionSymbol;
    class VariableSymbol;
}

namespace JVMProcessor {
    class FinalizedBody;
    class JAsmBuilder;
    enum class JVMOpCode;
}

namespace CodeGen {
class CodeGenPayload;
class MethodReference;

class CodeGenerator {
private:
    std::ostream* m_outstream;
    CodeGenPayload* m_payload;
    bool m_debug_mode;
    bool m_emit_part_four_info;
    JVMProcessor::JAsmBuilder* m_builder;
    std::unordered_map<Symbols::FunctionSymbol*, JVMProcessor::FinalizedBody*> m_compiled_method_bodies;
    std::vector<Binding::BoundVariableDeclarationNode*> m_fields_needing_clinit;
    std::vector<MethodReference*> m_resolved_method_references;
    std::unordered_map<std::string, MethodReference*> m_method_map;

    MethodReference* _putchar_method_reference;
    MethodReference* _getchar_method_reference;
    MethodReference* _to_char_array_method_reference;
    
    void init();
    void emit_required_methods();
    
    /**
     * @brief Because we don't need to actually emit all methods in our
     * resolved_method_references (e.g. java string lib stuff), we keep
     * track of what we actually need to emit here. 
     */
    std::vector<MethodReference*> m_methods_to_emit;
    
    /**
     * @brief It doesn't actually matter if the struct is supposed to be
     * global or local to a function since any attempts to access it outside
     * of scope get caught in the binder, and in the JVM there isn't a way
     * that I'm aware of that allows us to make "structs" (aka classes)
     * local to a single function.
     * 
     * @param struct_decl The struct declaration to emit.
     */
    void emit_struct_declaration(Binding::BoundStructDeclarationNode* struct_decl);
    void emit_method(MethodReference* method);
    void emit_field(Binding::BoundVariableDeclarationNode* field, bool is_static = false);
    void emit_basic_assembly_metadata(std::string class_name);
    
    // ====================
    //     Synthesis
    // ====================

    void synthesize_init_method();
    void synthesize_clinit_method();
    void synthesize_j_main_method();


    // ====================
    //     Statements
    // ====================

    void emit_statement(Binding::BoundStatementNode* statement);
    void emit_block_statement(Binding::BoundBlockStatementNode* block_statement);
    void emit_return_statement(Binding::BoundReturnStatementNode* return_statement);
    void emit_goto_statement(Binding::BoundGotoStatementNode* goto_statement);
    void emit_conditional_goto_statement(Binding::BoundConditionalGotoStatementNode* cond_goto_statement);
    void emit_label_statement(Binding::BoundLabelStatementNode* label_statement);
    void emit_variable_group_declaration(Binding::BoundVariableGroupDeclarationNode* var_group_decl);
    void emit_variable_declaration(Binding::BoundVariableDeclarationNode* variable_declaration);

    void emit_conditional_branch(Binding::BoundExpressionNode* condition, Binding::BoundLabel*& label, bool jmp_if_true);
    void emit_conditional_branch_bin_op(Binding::BoundExpressionNode*& expr, Binding::BoundLabel*& destination, bool jmp_if_true, bool& should_repeat);

    static JVMProcessor::JVMOpCode get_jump_code(Binding::BoundBinaryOperatorNode* bin_op, bool jmp_if_true, JVMProcessor::JVMOpCode& reverse_op_code);


    // ====================
    //     Expressions
    // ====================

    void emit_expression(Binding::BoundExpressionNode* expression, bool used);
    /**
     * @brief Guarded by emit_expression as to allow common optimizations before 
     * recursively descending down the bound tree.
     * 
     * @param expression The expression to emit.
     * @param used Whether or not the result of this expression is depended on.
     */
    void emit_expression_driver(Binding::BoundExpressionNode* expression, bool used);
    void emit_assignment_expression(Binding::BoundAssignmentExpressionNode* assignment, bool used);
    void emit_call_expression(Binding::BoundCallExpressionNode* call_expression, bool used);
    void emit_cast_expression(Binding::BoundCastExpressionNode* cast_expression, bool used);
    void emit_load(Binding::BoundVariableReferenceExpressionNode* variable_reference_expression);
    void emit_load(Symbols::VariableSymbol* variable_reference);
    void emit_store(Binding::BoundVariableReferenceExpressionNode* variable_reference_expression);
    void emit_store(Symbols::VariableSymbol* variable_reference);
    void emit_increment_expression(Binding::BoundIncrementExpressionNode* increment_expression, bool used);
    void emit_decrement_expression(Binding::BoundDecrementExpressionNode* decrement_expression, bool used);
    void emit_binary_expression(Binding::BoundBinaryExpressionNode* binary_expression, bool used);
    void emit_unary_expression(Binding::BoundUnaryExpressionNode* unary_expression, bool used);
    void emit_index_expression(Binding::BoundIndexExpressionNode* index_expression, bool used);

    // ====================
    //    Helper Methods
    // ====================

    /**
     * @brief Emit a pop instruction if the result of another instruction
     * goes unused.
     * 
     * @param used Whether or not the result from the current instruction will be used.
     */
    void emit_pop_if_unused(bool used);

    /**
     * @brief Adds a local variable symbol the builder's internal list.
     * 
     * @param local_variable The local variable to declare.
     */
    void declare_local(Symbols::VariableSymbol* local_variable);

    /**
     * @brief Resolves built-in (libc and init family) methods
     * along with all user defined methods. 
     */
    void resolve_methods();

    int calculate_call_stack_delta(Symbols::FunctionSymbol* method);

    Binding::BoundLabel* generate_temp_label() const;

    ///TODO: should probably refactor these methods into their own class or something.

    void add_method_to_map(MethodReference* method);
    void update_method_in_method_map(MethodReference* new_method);
    MethodReference* look_up_method(std::string name);
    MethodReference* look_up_method(std::string name, std::string owner);
    MethodReference* look_up_method(Symbols::FunctionSymbol* func);
public:
    CodeGenerator(CodeGenPayload* payload, bool debug, bool emit_part_four_info = false);

    bool emit();

    bool is_debug() const;
};

}

#endif