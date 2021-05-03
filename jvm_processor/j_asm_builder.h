#pragma once
#ifndef J_ASM_BUILDER_H
#define J_ASM_BUILDER_H

#include <cstdint>
#include <vector>
#include <string>

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

class JAsmBuilder {
private:
    BuilderState* m_builder_state;
    std::vector<Instruction*> m_instructions;
public:
    JAsmBuilder();

    void adjust_stack(int net_change);

    void emit_op_code(JVMOpCode op_code);
    void emit_op_code(JVMOpCode op_code, int stack_adj);
    void emit_op_code(JVMOpCode op_code, InstructionArgument* arg);
    void emit_op_code(JVMOpCode op_code, InstructionArgument* arg, int stack_adj);

    void emit_return_op_code(ReturnOpType ret_op_type = ReturnOpType::Void);
    void emit_branch_op_code(JVMOpCode op_code, InstructionArgument* arg);

    std::vector<std::string> current_instruction_listing() const;

    FinalizedBody* finalize();
};

}

#endif