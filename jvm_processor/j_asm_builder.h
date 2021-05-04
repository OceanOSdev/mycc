#pragma once
#ifndef J_ASM_BUILDER_H
#define J_ASM_BUILDER_H

#include <cstdint>
#include <vector>
#include <string>

namespace Symbols {
    class VariableSymbol;
}

namespace JVMProcessor {

class BuilderState;
class InstructionArgument;
class Instruction;
class FinalizedBody;

enum JVMOpCode : uint8_t;

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
    std::vector<LabelFixUpData*> m_fixups; // <-- todo: something with this, ill get back to it later.

    bool m_finalized;

    int get_local_variable_index(Symbols::VariableSymbol* local);
    void emit_local_load_or_store_int(int local_index, LSFlag flag = LSFlag::Load);
    void emit_local_load_or_store_float(int local_index, LSFlag flag = LSFlag::Load);
    void emit_local_load_or_store_reference(int local_index, LSFlag flag = LSFlag::Load);

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



    std::vector<std::string> current_instruction_listing() const;

    FinalizedBody* finalize();

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
};

}

#endif