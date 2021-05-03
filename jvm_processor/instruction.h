#pragma once
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>
#include <vector>

namespace JVMProcessor {

enum JVMOpCode : uint8_t;
class InstructionArgument;

class Instruction {
private:
    JVMOpCode m_op_code;
    std::vector<InstructionArgument*> m_arguments;
public:
    Instruction(JVMOpCode op_code, std::vector<InstructionArgument*> arguments);
    Instruction(JVMOpCode op_code, InstructionArgument* argument);
    Instruction(JVMOpCode op_code);

    /**
     * @brief The op code belonging to this instruction.
     */
    JVMOpCode op_code() const;

    /**
     * @brief The list of arguments taken by the op code in
     * this instruction.
     */
    std::vector<InstructionArgument*> arguments() const;

    /**
     * @brief The op code's mnemonic.
     */
    std::string op_code_str() const;

    /**
     * @brief This instruction's string representation.
     * 
     * @return A java assembly formatted string representation of this instruction.
     */
    std::string str() const;
};


}


#endif