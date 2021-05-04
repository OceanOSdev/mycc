#pragma once
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <fstream>
#include <ostream>
#include <iostream>
#include <unordered_map>

namespace Binding {
    class BoundFunctionDefinitionNode;
    class BoundStatementNode;
    class BoundBlockStatementNode;
    class BoundLabelStatementNode;
    class BoundGotoStatementNode;
    class BoundConditionalGotoStatementNode;
    class BoundReturnStatementNode;
    class BoundVariableGroupDeclarationNode;
    class BoundVariableDeclarationNode;
    class BoundStructDeclarationNode;

    class BoundExpressionNode;
}

namespace Symbols {
    class FunctionSymbol;
    class VariableSymbol;
}

namespace JVMProcessor {
    class FinalizedBody;
    class JAsmBuilder;
}

namespace CodeGen {
class CodeGenPayload;

class CodeGenerator {
private:
    std::ostream* m_outstream;
    CodeGenPayload* m_payload;
    bool m_debug_mode;
    JVMProcessor::JAsmBuilder* m_builder;
    std::unordered_map<Symbols::FunctionSymbol*, JVMProcessor::FinalizedBody*> m_compiled_method_bodies;
    

    void emit_global_variable_declaration(Symbols::VariableSymbol* global_variable);
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
    void emit_method(Binding::BoundFunctionDefinitionNode* method);

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

    // ====================
    //     Expressions
    // ====================

    void emit_expression(Binding::BoundExpressionNode* expression, bool used);

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

    

public:
    CodeGenerator(CodeGenPayload* payload, bool debug);

    bool emit();

    bool is_debug() const;
};

}

#endif