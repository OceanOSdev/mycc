#pragma once
#ifndef J_ASM_BUILDER_H
#define J_ASM_BUILDER_H

#include <cstdint>
#include <vector>
namespace JVMProcessor {

class BuilderState;
class InstructionArgument;
class Instruction;
enum JVMOpCode : uint8_t;

class JAsmBuilder {
private:
    BuilderState* m_builder_state;
    std::vector<Instruction*> m_instructions;
public:
    void emit_op_code(JVMOpCode op_code, InstructionArgument* arg);
    void emit_op_code(JVMOpCode op_code, InstructionArgument* arg, int stack_adj);
    void emit_op_code(JVMOpCode op_code, std::vector<InstructionArgument*> args);
    void emit_op_code(JVMOpCode op_code, std::vector<InstructionArgument*> args, int stack_adj);
};

}

#endif