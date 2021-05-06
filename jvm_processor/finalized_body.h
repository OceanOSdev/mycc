#pragma once
#ifndef FINALIZED_BODY_H
#define FINALIZED_BODY_H

#include <vector>
#include <string>

namespace JVMProcessor {
class Instruction;

/**
 * @brief The payload delivered by the JAsmBuilder after finalizing
 * the emitted instruction list.
 */
class FinalizedBody {
private:
    std::vector<Instruction*> m_body;
    std::vector<std::string> m_hard_coded_body;
    int m_local_count;
    int m_max_stack;
    bool m_is_hard_coded;
public:
    FinalizedBody(std::vector<Instruction*> body, int locals, int max_stack);
    FinalizedBody(std::vector<std::string> body, int locals, int max_stack);

    /**
     * @brief A list of the instructions.
     */
    std::vector<Instruction*> body() const;

    /**
     * @brief The list of strings representing the body of a hard
     * coded method.
     */
    std::vector<std::string> hard_coded_body() const;

    bool is_hard_coded() const;

    /**
     * @brief The number of locals declared in 
     * the body.
     * In the case of methods, this includes
     * parameters. 
     */
    int local_count() const;

    /**
     * @brief The max size the stack has to be to be
     * able to execute all instructions in the body.
     */
    int max_stack_size() const;
};

}

#endif