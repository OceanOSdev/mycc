#pragma once
#ifndef JVM_OPCODE_INFO_H
#define JVM_OPCODE_INFO_H

#include <cstdint>
#include <string>

namespace JVMProcessor {

enum JVMOpCode : uint8_t;

/**
 * @note Since it isn't necessary to make a whole class to house the methods pertaining
 * to getting jvm opcode metadata we just put them in a nested namespace as to not pollute
 * the @c JVMProcessor @endcode namespace with method signatures. 
 */
namespace OpCodeInfo {

/**
 * @brief Determines if the given opcode is a branch to a label.
 * 
 * @param op_code The opcode.
 * @return True if the opcode is a branch instruction.
 */
bool is_branch(JVMOpCode op_code);

/**
 * @brief Determines if the given opcode is a conditional dependent branch instruction.
 * 
 * @param op_code The opcode.
 * @return True if the opcode is a conditional branch.
 */
bool is_conditional_branch(JVMOpCode op_code);

/**
 * @brief Determines if the given opcode is a conditional based on a relation between variables.
 * 
 * @param op_code The opcode.
 * @return True if the opcode branches based of a relational condition.
 */
bool is_relational_branch(JVMOpCode op_code);

/**
 * @brief Determines how many items the given opcode pops off the stack machine.
 * 
 * @note We can return a signed int8 since the highest fixed stack difference
 *       will only be something around like 4 or something and we use -1 to
 *       represent when the opcode pops a variable number of items.
 * 
 * @param op_code The opcode.
 * @return How many items are popped off of the stack. 
 *         -1 if a variable amount are popped off.
 */
int8_t stack_pop_modifier(JVMOpCode op_code);

/**
 * @brief Determines how many items the given opcode pushes onto the stack machine.
 * 
 * @note We can return a signed int8 since the highest fixed stack difference
 *       will only be something around like 4 or something and we use -1 to
 *       represent when the opcode pushes a variable number of items.
 * 
 * @param op_code The opcode.
 * @return How many items are pushed onto the stack.
 *         -1 if a variable amount are pushed on.
 */
int8_t stack_push_modifier(JVMOpCode op_code);

/**
 * @brief Determines whether the given opcode always pushes/pops the same number items.
 * 
 * Opcodes like those in the return or invoke family modify the stack in ways that are
 * dependent on their context (e.g. given a stack with n items, return will pop n items)
 * and thus we cannot calculate how they will affect the stack "in a vacuum" so to say.
 * 
 * @param op_code The opcode.
 * @return True if the net stack modification by 'op_code' is independent from the
 *         current state of the stack when executed.
 */
bool is_constant_stack_modifier(JVMOpCode op_code);

/**
 * @brief Determines the overall stack behavior of the given opcode.
 * 
 * @param op_code The opcode.
 * @return The net difference in items on the stack machine after executing 'opcode'
 */
int net_stack_modification(JVMOpCode op_code);

/**
 * @brief The string representation of the opcode.
 * 
 * (i.e. its mnemonic).
 * 
 * @param op_code The opcode.
 * @return A string containing the opcode's mnemonic.
 */
std::string op_code_name(JVMOpCode op_code);

/**
 * @brief Determines if the given opcode transfers control away from a block.
 * 
 * Opcodes that satisfy this condition should only be at the end of any given
 * basic block.
 * 
 * @param op_code The opcode. 
 * @return True if this opcode transfers control away from a basic block.
 */
bool is_control_transfer(JVMOpCode op_code);

}


}

#endif