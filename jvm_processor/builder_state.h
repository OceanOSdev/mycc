#pragma once
#ifndef BUILDER_STATE_H
#define BUILDER_STATE_H

#include <stack>
#include <cstdint>

namespace JVMProcessor {

enum class JVMOpCode;

/**
 * @brief For debugging purposes, we create an enum to
 * keep track of what types gets pushed onto the jvm
 * stack during bytecode emitting. 
 * 
 */
enum StackMachineItem : char {
    reference = 'a',
    character = 'c',
    _float = 'f',
    integer = 'i'
};

/**
 * @brief A class to keep track of emitting and execution state
 * by a @c JAsmBuilder @endcode object.
 * 
 */
class BuilderState {
private:
    int m_max_stack_size;
    int m_current_stack_size;
    int m_op_codes_emitted;
    std::stack<StackMachineItem> m_current_stack_contents;
public:
    /**
     * @brief Creates a new BuilderState object with
     * all counters and stacks initalized to 0 (and empty).
     */
    BuilderState();

    /**
     * @brief The largest number of items that have been on
     * the stack at one time.
     */
    int max_stack_size() const;

    /**
     * @brief The current number of items on the stack. 
     */
    int current_stack_size() const;

    /**
     * @brief The total number of op codes emitted by the builder. 
     */
    int op_codes_emitted() const;

    /**
     * @brief Records the emitted op code to the content stack.
     * 
     * @param op_code The op code that was emitted.
     * 
     * @note We actually don't record what gets added *quite* yet.
     */
    void add_op_code(JVMOpCode op_code);

    /**
     * @brief The current stack of types in the execution state.
     */
    std::stack<StackMachineItem> current_stack_contents() const;

    /**
     * @brief Mutate the stack in accordance with the effects of the
     * currently emitted op code.
     * 
     * @param net_change The total change in stack size to record.
     */
    void adjust_stack(int net_change);
};

}

#endif