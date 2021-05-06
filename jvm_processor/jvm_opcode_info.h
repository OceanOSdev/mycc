#pragma once
#ifndef JVM_OPCODE_INFO_H
#define JVM_OPCODE_INFO_H

#include <cstdint>
#include <string>

namespace JVMProcessor {
enum InstructionArgumentKind : uint16_t;

enum class JVMOpCode;

/**
 * @note Since it isn't necessary to make a whole class to house the methods pertaining
 * to getting jvm op code metadata we just put them in a nested namespace as to not pollute
 * the @c JVMProcessor @endcode namespace with method signatures. 
 */
namespace OpCodeInfo {

/**
 * @brief Determines if the given op code is a branch to a label.
 * 
 * @param op_code The op code.
 * @return True if the op code is a branch instruction.
 */
bool is_branch(JVMOpCode op_code);

/**
 * @brief Determines if the given op code is a conditional dependent branch instruction.
 * 
 * @param op_code The op code.
 * @return True if the op code is a conditional branch.
 */
bool is_conditional_branch(JVMOpCode op_code);

/**
 * @brief Determines if the given op code is a conditional based on a relation between variables.
 * 
 * @param op_code The op code.
 * @return True if the op code branches based of a relational condition.
 */
bool is_relational_branch(JVMOpCode op_code);

/**
 * @brief Determines how many items the given op code pops off the stack machine.
 * 
 * @note We can return a signed int8 since the highest fixed stack difference
 *       will only be something around like 4 or something and we use -1 to
 *       represent when the op code pops a variable number of items.
 * 
 * @param op_code The op code.
 * @return How many items are popped off of the stack. 
 *         -1 if a variable amount are popped off.
 */
int8_t stack_pop_modifier(JVMOpCode op_code);

/**
 * @brief Determines how many items the given op code pushes onto the stack machine.
 * 
 * @note We can return a signed int8 since the highest fixed stack difference
 *       will only be something around like 4 or something and we use -1 to
 *       represent when the op code pushes a variable number of items.
 * 
 * @param op_code The op code.
 * @return How many items are pushed onto the stack.
 *         -1 if a variable amount are pushed on.
 */
int8_t stack_push_modifier(JVMOpCode op_code);

/**
 * @brief Determines whether the given op code always pushes/pops the same number items.
 * 
 * Opcodes like those in the return or invoke family modify the stack in ways that are
 * dependent on their context (e.g. given a stack with n items, return will pop n items)
 * and thus we cannot calculate how they will affect the stack "in a vacuum" so to say.
 * 
 * @param op_code The op code.
 * @return True if the net stack modification by 'op_code' is independent from the
 *         current state of the stack when executed.
 */
bool is_constant_stack_modifier(JVMOpCode op_code);

/**
 * @brief Determines the overall stack behavior of the given op code.
 * 
 * @param op_code The op code.
 * @return The net difference in items on the stack machine after executing 'op code'
 */
int net_stack_modification(JVMOpCode op_code);

/**
 * @brief The string representation of the op code.
 * 
 * (i.e. its mnemonic).
 * 
 * @param op_code The op code.
 * @return A string containing the op code's mnemonic.
 */
std::string op_code_name(JVMOpCode op_code);

/**
 * @brief Determines if the given op code transfers control away from a block.
 * 
 * Opcodes that satisfy this condition should only be at the end of any given
 * basic block.
 * 
 * @param op_code The op code. 
 * @return True if this op code transfers control away from a basic block.
 */
bool is_control_transfer(JVMOpCode op_code);

/**
 * @brief Determines the argument kind the given op code expects.
 * 
 * @param op_code The op code.
 * @return The expected argument kind.
 */
InstructionArgumentKind required_argument_kind(JVMOpCode op_code);

}


}

#endif