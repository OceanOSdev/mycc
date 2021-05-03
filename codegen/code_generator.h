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
    class BoundReturnStatementNode;

    class BoundExpressionNode;
}

namespace Symbols {
    class FunctionSymbol;
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

    void emit_global_variable_declarations();
    void emit_global_struct_declarations();
    void emit_local_struct_declarations();
    void emit_method(Binding::BoundFunctionDefinitionNode* method);

    // ====================
    //     Statements
    // ====================

    void emit_statement(Binding::BoundStatementNode* statement);
    void emit_block_statement(Binding::BoundBlockStatementNode* block_statement);
    void emit_return_statement(Binding::BoundReturnStatementNode* return_statement);

    // ====================
    //     Expressions
    // ====================

    void emit_expression(Binding::BoundExpressionNode* expression);

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

public:
    CodeGenerator(CodeGenPayload* payload, bool debug);

    bool emit();

    bool is_debug() const;
};

}

#endif