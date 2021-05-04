#pragma once
#ifndef FINALIZED_BODY_H
#define FINALIZED_BODY_H

#include <vector>
#include <string>

namespace JVMProcessor {

/**
 * @brief The payload delivered by the JAsmBuilder after finalizing
 * the emitted instruction list.
 */
class FinalizedBody {
private:
    std::vector<std::string> m_body;
    int m_local_count;
    int m_max_stack;
public:
    FinalizedBody(std::vector<std::string> body, int locals, int max_stack);

    /**
     * @brief A list of stringified instructions.
     */
    std::vector<std::string> body() const;

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