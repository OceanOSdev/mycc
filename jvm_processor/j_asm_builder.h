#pragma once
#ifndef J_ASM_BUILDER_H
#define J_ASM_BUILDER_H

#include <cstdint>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

namespace Binding {
    class BoundVariableDeclarationNode;
    class BoundLiteralValExpressionNode;
    class BoundLabel;
}

namespace Symbols {
    class VariableSymbol;
    class FunctionSymbol;
}

namespace JVMProcessor {

class BuilderState;
class InstructionArgument;
class Instruction;
class FinalizedBody;

enum class JVMOpCode;
enum BlockDirectiveType : uint8_t {
    ClassDirective,
    MethodDirective,
    CodeDirective
};

enum ReturnOpType : uint8_t {
    Void,
    Integer,
    Float,
    Object 
};

class LabelFixUpData;

class JAsmBuilder {
private:
    /**
     * @brief JVM doesn't like it when we use these
     * as identifiers for things but since they
     * are all valid identifiers in our subset of
     * C, we just need to put quotations around
     * them should they show up in source code
     * that gets fed to the compiler.
     */
    static const std::set<std::string> m_forbidden_words;

    static std::string lit_as_emittable_const(Binding::BoundLiteralValExpressionNode* lit);

    /**
     * @brief Enum for denoting whether emit the
     * load or store op code in the 'emit_local_load_or_store_*'
     * family. I know I could just use a bool but for some reason
     * I'm really indecisive so I'm setting for this.
     * 
     * Private submember because we don't (and shouldn't) use this
     * anywhere other than the previously mentioned methods. 
     */
    enum LSFlag : uint8_t {
        Load,
        Store
    };
    BuilderState* m_builder_state;
    std::vector<Instruction*> m_instructions;
    std::vector<Symbols::VariableSymbol*> m_locals;
    std::vector<LabelFixUpData*> m_fixups;
    std::unordered_map<Binding::BoundLabel*, int> m_label_instr_map; 
    
    bool m_finalized;

    int get_local_variable_index(Symbols::VariableSymbol* local);
    void emit_local_load_or_store_int(int local_index, LSFlag flag = LSFlag::Load);
    void emit_local_load_or_store_float(int local_index, LSFlag flag = LSFlag::Load);
    void emit_local_load_or_store_reference(int local_index, LSFlag flag = LSFlag::Load);

    JVMOpCode op_code_for_emit_int_value(int value);


    void mark_branch_for_fix_up(int instruction_idx, Binding::BoundLabel* label);
    void track_label(Binding::BoundLabel* label, int instruction_idx);
    void run_label_fixes();

    /**
     * @brief Should be the primary way to record instructions so
     * that the builder state does not get out of sync with 
     * the instruction list.
     * 
     * @param instruction The instruction to record.
     */
    void record_instruction(Instruction* instruction);

public:
    JAsmBuilder();

    void adjust_stack(int net_change);

    void emit_op_code(JVMOpCode op_code);
    void emit_op_code(JVMOpCode op_code, int stack_adj);
    void emit_op_code(JVMOpCode op_code, InstructionArgument* arg);
    void emit_op_code(JVMOpCode op_code, InstructionArgument* arg, int stack_adj);

    void emit_return_op_code(ReturnOpType ret_op_type = ReturnOpType::Void);
    void emit_branch_op_code(JVMOpCode op_code, InstructionArgument* arg);

    void emit_local_load(Symbols::VariableSymbol* local);
    void emit_local_store(Symbols::VariableSymbol* local);

    /**
     * @brief Emits a field load instruction to the instructions list.
     * 
     * Since we use static fields to emulate global variables, we need to 
     * create a psuedo-struct with the name of the source file (w/o the file extension)
     * so that we can access the static field of emitted "class". 
     * 
     * @param global The global variable symbol.
     * @param containing_class_name The base filename of the source file that defines the global.
     */
    void emit_global_load(Symbols::VariableSymbol* global, std::string containing_class_name);
    void emit_global_store(Symbols::VariableSymbol* global, std::string containing_class_name); // <-- similar situation

    void emit_constant(int arg);
    void emit_constant(float arg);
    void emit_constant(std::string arg);


    std::vector<std::string> current_instruction_listing() const;

    FinalizedBody* finalize();

    /**
     * @brief The number of instructions the builder has emit. 
     */
    int instructions_emitted() const;

    /**
     * @brief The current stack size.
     */
    int current_stack_size() const;

    /**
     * @brief Tell the emitter to track this label for future
     * potential fix ups.
     * 
     * @param label The label to mark.
     */
    void mark_label(Binding::BoundLabel* label);

    /**
     * @brief The largest number of items that have been on
     * the stack at one time during execution of body instructions.
     * 
     * Should only be called after finalize.
     */
    int max_stack_size() const;

    /**
     * @brief Adds a local variable symbol the builder's internal list.
     * 
     * @param local_variable The local variable to declare.
     */
    void declare_local(Symbols::VariableSymbol* local_variable);

    /**
     * @brief Emits a string representation of the class directive in
     * Java assembly.
     * 
     * @param class_name The name of the class in the assembly info.
     * @return '.class public "class_name"'
     */
    static std::string emit_class_directive(std::string class_name);

    /**
     * @brief Emits a string representation of the super directive.
     * 
     * @return '.super java/lang/Object'
     */
    static std::string emit_super_directive();

    /**
     * @brief Emits a string representation of the field directive.
     * 
     * @param field_decl The variable declaration for the field.
     * @param is_static Whether or not this is a static field 
     *      (in our case, a global variable vs a struct member).
     * @param requires_clinit An out-param that notifies the caller if the field needs
     *       to be initialized in the <clinit> method.
     * @return '.field public [static] [final] <name> <Type> [= <init_expr>]'
     */
    static std::string emit_field_directive(Binding::BoundVariableDeclarationNode* field_decl, bool is_static, bool& requires_clinit);
    
    /**
     * @brief Emits a string representation of the method directive.
     * 
     * @param method The function symbol for this method.
     * @param is_static Whether or not this is a special function (i.e an initializer).
     * @return '.method public [static] <name> : ([<Type list>])<Type>'
     */
    static std::string emit_method_definition_header(Symbols::FunctionSymbol* method, bool is_static = true);
    
    /**
     * @brief Emits a string representation of the code directive.
     * 
     * @param max_stack_size The size the stack becomes in this method.
     * @param local_count The total number of locals including parameters.
     * @return '.code stack <max_stack_size> locals <local_count>'
     */
    static std::string emit_method_code_directive(int max_stack_size, int local_count);
    
    /**
     * @brief Emits a string that ends a block directive.
     * 
     * @param directive_type The type of directive to "end".
     * @return '.end <directive_type>'
     */
    static std::string emit_directive_end(BlockDirectiveType directive_type);


    /**
     * @brief Wraps a string with quotations if need be so that
     * it can be used as an identifier by the Krakatau Java
     * assembler.
     * 
     * @param str The string to wrap (should we need to).
     */
    static std::string str_as_emittable_identifier(std::string str);
};

}

#endif